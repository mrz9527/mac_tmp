/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
* Description: dict maker
* Author: ZhouKang
* Create: 2021-7-12
*/
#include "MakeBinDict.h"
#include "FreqConvertScore.h"
#include "Log.h"
#include "securecutil.h"

BEGIN_NAMESPACE_CHINESE_IME

void MakeBinDict::AllocResource(size_t n)
{
    m_vecDictWordItem.reserve(n);
}

void MakeBinDict::ReleaseResource()
{
    if (!m_vecDictWordItem.empty()) {
        std::vector<DictWordItem>().swap(m_vecDictWordItem);
    }
}

void MakeBinDict::SetDictVersionAndBrief(const char* version, size_t versionLen, const char16* brief, size_t briefLen)
{
    m_dictHeader.Init();

    // 记录词头前256个字节信息
    if (version != nullptr) {
        size_t versionSize = versionLen * sizeof(char);
        versionSize = versionSize > static_cast<size_t>(DictHeadSize::DICT_HEADER_VERSION_SIZE) ? static_cast<size_t>(DictHeadSize::DICT_HEADER_VERSION_SIZE) : versionSize;
        memcpy_s((void*)m_dictHeader.version, versionSize, (void*)version, versionSize);
    }

    if (brief != nullptr) {
        size_t briefSize = briefLen * sizeof(char16);
        briefSize = briefSize > static_cast<size_t>(DictHeadSize::DICT_HEADER_BRIEF_SIZE) ? static_cast<size_t>(DictHeadSize::DICT_HEADER_BRIEF_SIZE) : briefSize;
        memcpy_s((void*)m_dictHeader.brief, briefSize, (void*)brief, briefSize);
    }
}

bool MakeBinDict::MakeProcess(const char* txtDictPath, const char* binDictPath, DictType dictType)
{
    if (txtDictPath == nullptr || binDictPath == nullptr || dictType == DictType::DICT_TYPE_UNKOWN) {
        LOGW("txtDictPath == nullptr || binDictPath == nullptr || dictType == DICT_TYPE_UNKOWN");
        return false;
    }

    std::string srcPath(txtDictPath);
    std::string destPath(binDictPath);

    if (Utils::IsFileOrDir(srcPath) != 2) { // 0 不存在，1文件夹， 2文件
        LOGW("Utils::IsFileOrDir(srcPath) != 2");
        return false;
    }

    int pathType2 = Utils::IsFileOrDir(binDictPath);
    if (pathType2 == 0) {  // 如果path1是文件，path2不存在，则以path2为路径创建文件名
        pathType2 = 2;
    }

    srcPath = Utils::RelPath2AbsPath(txtDictPath);
    destPath = Utils::RelPath2AbsPath(binDictPath);

    std::string file(srcPath);

    std::string::size_type pos1 = file.rfind("/");
    std::string fileName = file.substr(pos1 + 1, std::string::npos);
    if (!SetDictMemoryLayout(dictType)) {
        LOGW("SetDictMemoryLayout(dictType) failed");
        return false;
    }

    // 加载词表文件
    size_t utf16bufSize = 0;
    char16* utf16buf = ReadWordTableFile(file.c_str(), utf16bufSize);
    if (nullptr == utf16buf) {
        LOGW("%s:ReadWordTableFile failed", fileName.c_str());
        return false;
    }

    ReleaseResource();
    if (!ParseWordTable(utf16buf, utf16bufSize)) {
        LOGW("%s:ParseWordTable failed", fileName.c_str());
        free(utf16buf);
        utf16buf = nullptr;
        return false;
    }

    free(utf16buf);
    utf16buf = nullptr;

    std::string predictDictFile(destPath);
    std::string binDictFile(destPath);
    if (pathType2 == 1) {
        std::string::size_type pos = fileName.rfind(".");
        if (pos == std::string::npos) {
            predictDictFile += std::string("/") + fileName + std::string("_predict.dat");
            binDictFile += std::string("/") + fileName + std::string(".dat");
        } else {
            predictDictFile +=
                    "/" + fileName.replace(pos, fileName.size() - pos, "_predict.dat");
            binDictFile += "/" + fileName.replace(pos, fileName.size() - pos, ".dat");
        }
    }

    MakePredict(predictDictFile.c_str());

    FillDictHeader();

    if (false == WriteDictFile(binDictFile.c_str())) {
        LOGW("%s:WriteDictFile failed", binDictFile.c_str());
        return false;
    }

    return true;
}

char16* MakeBinDict::ReadWordTableFile(const char* wordTableFile, size_t &utf16bufLen)
{
    if (wordTableFile == nullptr)
        return nullptr;

    // 读词表文件
    FILE* fp = fopen(wordTableFile, "rb");
    if (fp == nullptr)
        return nullptr;

    // 校验utf16-le BOM
    char16 utf16header;
    if (fread(&utf16header, sizeof(char16), 1, fp) != 1 || utf16header != 0xfeff) {
        fclose(fp);
        return nullptr;
    }

    // 计算文件长度，并读取文件内容到缓冲区
    fseek(fp, 0, SEEK_END);
    size_t bytes = ftell(fp) - 2; // 得到有效字节长度
    fseek(fp, 2, SEEK_SET);

    if (bytes == 0 || bytes % 2 != 0) { // utf16格式，文件字节数应该是偶数
        fclose(fp);
        return nullptr;
    }

    // 分配utf16缓冲区
    utf16bufLen = bytes / 2 + 1; // 计算utf16字符数， +1 是结尾符(char16)'\0'
    char16* utf16buf = (char16*) malloc(utf16bufLen * 2);
    if (utf16buf == nullptr) {
        fclose(fp);
        return nullptr;
    }
    memset_s(utf16buf, utf16bufLen * sizeof(char16), 0, utf16bufLen * sizeof(char16));

    fread(utf16buf, sizeof(char16), utf16bufLen, fp);
    fclose(fp);

    return utf16buf;
}

bool MakeBinDict::ParseWordTable(char16* utf16buf, size_t utf16bufLen)
{
    if (utf16buf == nullptr || utf16bufLen == 0) {
        return false;
    }

    AllocResource();
    std::vector<DictWordItem> &vecDictWordItem = m_vecDictWordItem;
    DictMemoryLayout dictMemLayout(m_dictHeader.memLayout);

    std::vector<double> vecFreqDouble;
    //
    char16* cur_utf16buf = utf16buf;
    while (cur_utf16buf && *cur_utf16buf != (char16) '\0') {
        DictWordItem dictWordItem;
        std::vector<std::string> vecKeyStr;
        // 关键字
        if (!MakeBinDict::ExtractKeyStr(cur_utf16buf, &cur_utf16buf, vecKeyStr) ||
            vecKeyStr.size() == 0) {
            LOGW("ExtractKeyStr exception");
            MakeBinDict::JumpToNextLine(cur_utf16buf, &cur_utf16buf); // 异常，跳到下一行
            continue;
        }

        if (cur_utf16buf == nullptr) // 条件成立时，break，后续没有词和词频
            break;

        // key1 和 key2
        if (dictMemLayout.key1StrOcc == 0 &&
            dictMemLayout.key2StrOcc > 0) { // 笔画，此时是数字键，放到key2上
            dictWordItem.vecNumber.swap(vecKeyStr);
        } else if (dictMemLayout.key1StrOcc > 0 && dictMemLayout.key2StrOcc == 0) {
            dictWordItem.vecPy.swap(vecKeyStr);
        } else if (dictMemLayout.key1StrOcc > 0 &&
                   dictMemLayout.key2StrOcc > 0) { // keyTypeCount = 2
            int keyStrSize = vecKeyStr.size();
            if (keyStrSize % 2 != 0) {
                LOGW("keyStrSize %% 2 != 0");
                MakeBinDict::JumpToNextLine(cur_utf16buf, &cur_utf16buf); // 异常，跳到下一行
                continue;
            }
            dictWordItem.vecPy.insert(dictWordItem.vecPy.end(), vecKeyStr.begin(),
                                      vecKeyStr.begin() + keyStrSize / 2);
            dictWordItem.vecNumber.insert(dictWordItem.vecNumber.end(),
                                          vecKeyStr.begin() + keyStrSize / 2,
                                          vecKeyStr.end());
        } else {
            LOGW("dictMemLayout.key1StrOcc == 0 && dictMemLayout.key2StrOcc == 0");
            return false;
        }

        //词和词频：提取
        std::vector<std::u16string> vecWordAndFreq;
        if (!MakeBinDict::ExtractWordAndFreq(cur_utf16buf, &cur_utf16buf, vecWordAndFreq) ||
            vecWordAndFreq.size() == 0 || vecWordAndFreq.size() % 2 != 0) {
            LOGW("ExtractWordAndFreq error");
            MakeBinDict::JumpToNextLine(cur_utf16buf, &cur_utf16buf); // 异常，跳到下一行
            continue;
        }

        // 用于校验
        uint8 ziCount = dictMemLayout.key2StrOcc > 0 ? dictWordItem.vecNumber.size()
                                                     : dictWordItem.vecPy.size();
        std::u16string token;
        for (int i = 0; i < vecWordAndFreq.size(); i += 2) {
            token = vecWordAndFreq[i];
            if (dictMemLayout.toneStrOcc > 0) {
                if (token.size() != 2 * ziCount) {
                    LOGW("dictMemLayout.toneStrOcc>0, token.size() != 2 * ziCount");
                    continue;
                }
            } else {
                if (token.size() != ziCount) {
                    LOGW("token.size() != ziCount");
                    continue;
                }
            }
            dictWordItem.vecWordAndFreq.emplace_back(std::make_pair(token, 0)); //  词频先填充0
            token = vecWordAndFreq[i + 1];
            vecFreqDouble.emplace_back(
                    Utf16Atof(token.c_str()));
        }
        dictWordItem.AdjustResource();
        vecDictWordItem.emplace_back(std::move(dictWordItem));
    }

    // 找出容器的最大词频和最小词频，用于归一化
    std::vector<double>::iterator maxFreqIter = std::max_element(vecFreqDouble.begin(),
                                                                 vecFreqDouble.end());
    std::vector<double>::iterator minFreqIter = std::min_element(vecFreqDouble.begin(),
                                                                 vecFreqDouble.end());

    std::vector<double>::iterator curIter = vecFreqDouble.begin();
    std::vector<double>::iterator endIter = vecFreqDouble.end();

    // 放大系数
    FreqConvertScore::CalcAmplifierCoeff(*minFreqIter, *maxFreqIter, 65535);
    for (auto &dictWordItem:vecDictWordItem) {
        // double类型词频转uint16词频
        for (auto &wordAndFreq : dictWordItem.vecWordAndFreq) {
            if (curIter <= endIter)
                wordAndFreq.second = FreqConvertScore::Standardization(*curIter++);
            else
                return false;
        }
        // 内部词频排序
        dictWordItem.SortByFreq();
    }

    return true;
}

void MakeBinDict::FillDictHeader()
{
    m_dictHeader.wordItemCount = m_vecDictWordItem.size();

    // 记录词典主体信息
    uint32 fileSize = static_cast<uint32>(DictHeadSize::DICT_HEADER_SIZE); // 从偏移位置512开始
    DictMemoryLayout dictMemLayout(m_dictHeader.memLayout);

    for (auto dictWordItem : m_vecDictWordItem) {
        // 一个词的字数占用空间
        fileSize += dictMemLayout.ziCountOcc;

        // key1字符串长度占用空间
        fileSize += dictMemLayout.key1LenOcc;

        // key2字符串长度占用空间
        fileSize += dictMemLayout.key2LenOcc; //

        // key1字符串占用空间
        if (dictMemLayout.key1StrOcc) { // 笔画特殊处理：如果是笔画，进入隐含的else分支，不记录key1
            for (auto py : dictWordItem.vecPy) {
                fileSize += py.size() * dictMemLayout.key1StrOcc + 1;
            }
        }

        // key2字符串占用空间
        if (dictMemLayout.key2StrOcc)
            for (auto number : dictWordItem.vecNumber) {
                fileSize += number.size() * dictMemLayout.key2StrOcc + 1;
            }

        // 词数占用空间
        fileSize += dictMemLayout.wordCountOcc;

        // 占用空间：词和词频占用字节大小
        uint32 ziCount =
                dictMemLayout.key2StrOcc > 0 ? dictWordItem.vecNumber.size()
                                             : dictWordItem.vecPy.size();
        uint32 wordCount = dictWordItem.vecWordAndFreq.size();
        fileSize +=
                wordCount * (ziCount * (dictMemLayout.wordStrOcc + dictMemLayout.toneStrOcc) +
                             dictMemLayout.freqOcc);
    }

    m_dictHeader.fileSize = fileSize;
}

bool MakeBinDict::WriteDictFile(const char* dictFilePath) const
{
    if (dictFilePath == nullptr) {
        LOGW("dictFilePath == nullptr");
        return false;
    }

    if (m_dictHeader.fileSize < static_cast<uint32>(DictHeadSize::DICT_HEADER_SIZE)) {
        LOGW("m_dictHeader.fileSize < DICT_HEADER_SIZE");
        return false;
    }

    char* buf = (char*) malloc(m_dictHeader.fileSize);
    if (buf == nullptr) {
        LOGW("buf == nullptr");
        return false;
    }
    memset_s(buf, m_dictHeader.fileSize, 0, m_dictHeader.fileSize);

    // 词头，0~255字节
    size_t pos = 0;

    size_t bytes = sizeof(m_dictHeader.version);
    memcpy_s((void*)(buf + pos), bytes, (void*)&m_dictHeader.version, bytes); // 版本号
    pos += bytes;

    bytes = sizeof(m_dictHeader.fileSize);
    memcpy_s((void*)(buf + pos), bytes, (void*)&m_dictHeader.fileSize, bytes); // 文件大小
    pos += bytes;

    bytes = sizeof(m_dictHeader.brief);
    memcpy_s((void*)(buf + pos), bytes, (void*)&m_dictHeader.brief, bytes); // 简介信息
    pos += bytes;

    bytes = sizeof(m_dictHeader.wordItemCount);
    memcpy_s((void*)(buf + pos), bytes, (void*)&m_dictHeader.wordItemCount, bytes);
    pos += bytes;

    bytes = sizeof(m_dictHeader.memLayout);
    memcpy_s((void*)(buf + pos), bytes, (void*)&m_dictHeader.memLayout, bytes);
    pos += bytes;

    // 词主体部分
    pos = static_cast<size_t>(DictHeadSize::DICT_HEADER_SIZE);
    DictMemoryLayout dictMemLayout(m_dictHeader.memLayout);

    // 词典主体部分信息
    for (auto &dictWordItem:m_vecDictWordItem) {
        // 存储一个词的字数
        uint8 ziCount = static_cast<uint8>(dictMemLayout.key2StrOcc > 0
                                           ? dictWordItem.vecNumber.size()
                                           : dictWordItem.vecPy.size());
        if (dictMemLayout.ziCountOcc > 0) {
            bytes = sizeof(ziCount);
            memcpy_s((void*)(buf + pos), bytes, (void*)&ziCount, bytes);
            pos += bytes;
        }

        if (dictMemLayout.key1LenOcc > 0) {
            uint8 key1Len = 0;
            for (auto &py:dictWordItem.vecPy) {
                key1Len += py.size();
            }
            bytes = sizeof(key1Len);
            memcpy_s((void*)(buf + pos), bytes, (void*)&key1Len, bytes); // key1 的长度
            pos += bytes;
        }

        // 存储key2的长度
        if (dictMemLayout.key2LenOcc > 0) {
            uint8 numberLen = 0;
            for (auto &number:dictWordItem.vecNumber) {
                numberLen += number.size();
            }
            bytes = sizeof(numberLen);
            memcpy_s((void*)(buf + pos), bytes, (void*)&numberLen, bytes); // key1 的长度
            pos += bytes;
        }

        char spaceCh = ' ';
        // 存储key1字符串
        if (dictMemLayout.key1StrOcc > 0) {
            for (auto &py:dictWordItem.vecPy) {
                bytes = py.size();
                memcpy_s((void*)(buf + pos), bytes, (void*)py.c_str(), bytes); // 拼音字符串
                pos += bytes;

                bytes = sizeof(spaceCh);
                memcpy_s((void*)(buf + pos), bytes, (void*)&spaceCh, bytes); // 空格
                pos += bytes;
            }
        }

        // 存储key2字符串
        if (dictMemLayout.key2StrOcc > 0) {
            for (auto &number:dictWordItem.vecNumber) {
                bytes = number.size();
                memcpy_s((void*)(buf + pos), bytes, (void*)number.c_str(), bytes); // 数字字符串
                pos += bytes;

                bytes = sizeof(spaceCh);
                memcpy_s((void*)(buf + pos), bytes, (void*)&spaceCh, bytes); // 空格
                pos += bytes;
            }
        }

        // 存储词的个数
        if (dictMemLayout.wordCountOcc > 0) {
            uint16 wordCount = static_cast<uint16> (dictWordItem.vecWordAndFreq.size());
            bytes = sizeof(wordCount);
            memcpy_s((void*)(buf + pos), bytes, (void*)&wordCount, bytes); // 词的个数
            pos += bytes;
        }

        // 存储词和词频（注音需要特殊处理，有音调
        for (auto &wordAndFreq : dictWordItem.vecWordAndFreq) {
            const std::u16string &word = wordAndFreq.first;
            const uint16 &freq = wordAndFreq.second;

            // 词字符串
            if (dictMemLayout.wordStrOcc > 0) {
                bytes = sizeof(char16) * ziCount;
                memcpy_s((void*)(buf + pos), bytes, (void*)word.c_str(), bytes); // 词
                pos += bytes;
            }

            // 音节字符串
            if (dictMemLayout.toneStrOcc > 0) { // 注音特殊处理
                size_t off = ziCount;
                while (off < word.size()) {
                    char toneCh = static_cast<char>(word[off]);
                    memcpy_s((void*)(buf + pos), 1, (void*)&toneCh, 1); // 音调
                    pos += 1;
                    ++off;
                }
            }

            // 词频
            if (dictMemLayout.freqOcc > 0) {
                bytes = sizeof(freq);
                memcpy_s((void*)(buf + pos), bytes, (void*)&freq, bytes); // 词频
                pos += bytes;
            }
        }
    }

    if (pos != m_dictHeader.fileSize) {
        LOGW("pos != m_dictHeader.fileSize");
        return false;
    }

    FILE* fp = fopen(dictFilePath, "wb");
    if (fp == nullptr) {
        free(buf);
        buf = nullptr;

        return false;
    } else {
        fwrite(buf, sizeof(char), pos, fp);
        fclose(fp);

        free(buf);
        buf = nullptr;

        return true;
    }
    return true;
}

MakeBinDict::~MakeBinDict()
{
    ReleaseResource();
}

bool
MakeBinDict::ExtractKeyStr(char16* str, char16** nextStr, std::vector<std::string> &vecKeyStr)
{
    bool correctFormat = true;

    if (!vecKeyStr.empty()) {
        std::vector<std::string>().swap(vecKeyStr);
    }

    if (nullptr == str || nullptr == nextStr) {
        return correctFormat;
    }

    char16* p = str;
    // 跳过空白字符
    while ((char16) ' ' == (*p) || (char16) '\t' == (*p) || (char16) '\r' == (*p) ||
           (char16) '\n' == (*p))  // mac的换行是\r，linux的换行是\n，windows的换行是\r\n
        ++p;

    *nextStr = p;
    std::string keyStr;
    while ((char16) '\0' != (*p) || (char16) '\r' == (*p) || (char16) '\n' == (*p)) {
        if ((char16) ' ' == (*p) || (char16) '\t' == (*p)) { // 分割标志
            *p = (char16) '\0';
            vecKeyStr.push_back(keyStr);
            std::string().swap(keyStr);
            ++p;

            // 避免词表格式不正确，存在连续多个空白字符
            while ((char16) ' ' == (*p) || (char16) '\t' == (*p))
                ++p;

            *nextStr = p;
        } else {                                             // 非分割标志
            char highChar = static_cast<char> ((*p) >> 8);
            char lowChar = static_cast<char> (*p);

            if (highChar == 0) {                               //  关键字
                keyStr.push_back(lowChar);
                ++p;
            } else {                                            //  不是关键字
                if (p > (*nextStr)) { // guo94664 strGroup = "guo94664"; p = "94664" p>strGroup
                    correctFormat = false;
                }
                break;
            }
        }
    }

    if (correctFormat && !keyStr.empty()) { // zhong guo ,查找到结尾guo，此时添加最后一个group
        vecKeyStr.emplace_back(keyStr);
        *nextStr = nullptr;
    }

    return correctFormat;
}

bool MakeBinDict::ExtractWordAndFreq(
        char16* str, char16** nextStr,
        std::vector<std::u16string> &vecWordAndFreq)
{
    if (!vecWordAndFreq.empty()) {
        std::vector<std::u16string>().swap(vecWordAndFreq);
    }

    if (nullptr == str || nullptr == nextStr) {
        return false;
    }

    char16* p = str;
    // 跳过空白字符，不包含换行符，因为此时已经解析到了词和词频，他们是跟在关键字后面的
    while ((char16) ' ' == (*p) || (char16) '\t' == (*p))
        ++p;

    char16* token = p;
    while ((char16) '\0' != (*p)) {
        if ((char16) '\r' == (*p) || (char16) '\n' == (*p)) { // 遇到换行，break
            *p = (char16) '\0';
            vecWordAndFreq.emplace_back(
                    std::u16string(token, p - token));
            *nextStr = ++p;

            return true;
        } else if ((char16) ' ' == (*p) || (char16) '\t' == (*p)) { // 遇到正常空白符' '和'\t'
            *p = (char16) '\0';
            vecWordAndFreq.emplace_back(
                    std::u16string(token, p - token));
            ++p;

            // 避免词表格式不正确，存在连续多个正常空白字符
            while ((char16) ' ' == (*p) || (char16) '\t' == (*p))
                ++p;

            token = p; // 更新token位
        } else { // 词字符或词频字符
            ++p;
        }
    }

    if (p > token) { // 最后一行，没有换行符，到达了'\0'终点
        vecWordAndFreq.emplace_back(
                std::u16string(token, p - token));
    }

    if ((char16) '\0' == (*p)) {
        *nextStr = nullptr;
    }

    if (vecWordAndFreq.empty()) {
        LOGW("vecWordAndFreq empty");
        return false;
    }
    if (vecWordAndFreq.size() % 2 != 0) {
        LOGW("vecWordAndFreq.size() %% 2 != 0");
        return false;
    }

    return true;
}

void MakeBinDict::JumpToNextLine(char16* str, char16** nextStr)
{
    if (nullptr == str || nullptr == nextStr) {
        return;
    }

    char16* p = str;
    while (*p != (char16) '\0') {
        if (*p == (char16) '\r') {
            ++p;
            if (*p == (char16) '\n')
                ++p;
            break;
        } else if (*p == (char16) '\n') {
            ++p;
            break;
        }
        ++p;
    }

    if (*p != (char16) '\0')
        *nextStr = nullptr;
    else
        *nextStr = p;
}

bool MakeBinDict::SetDictMemoryLayout(DictType dictType)
{
    return m_dictHeader.memLayout.SetMemLayout(dictType);
}

bool MakeBinDict::MakePredict(const char* predictDictFile, bool enablePredict)
{
    if (!enablePredict) {
        return true;
    }

    std::vector<std::pair<std::u16string, uint16>> pairs;
    for (const auto &dictWordItem:m_vecDictWordItem) {
        uint8 ziCount =
                m_dictHeader.memLayout.key2StrOcc > 0 ? dictWordItem.vecNumber.size()
                                                      : dictWordItem.vecPy.size();
        if (ziCount <= 1) { // 通过词表构建预测词的trie树，过滤1字词，1字词不参与构建树
            continue;
        }
        for (const auto &wordAndFreq:dictWordItem.vecWordAndFreq) {
            std::u16string word = wordAndFreq.first;
            if (m_dictHeader.memLayout.toneStrOcc > 0) {
                word = word.substr(0, ziCount);
            }
            pairs.push_back(std::make_pair(word, wordAndFreq.second));
        }
    }

    // 排序
    std::stable_sort(pairs.begin(), pairs.end(),
                     [](
                             const std::pair<std::u16string, uint16> &left,
                             const std::pair<std::u16string, uint16> &right) -> bool {
                         return left.first.compare(right.first) < 0;
                     });

    Predict<char16, uint16, Utf16Strlen> predict;
    if (predict.Build(pairs)) {
        predict.Save(predictDictFile);
    }

    return true;
}

END_NAMESPACE_CHINESE_IME