// author : zhoukang
// date   : 2022-08-26 23:46:13

#include "VertexData.h"
#include <fstream>

VertexData::VertexData(std::string vertexDataPath, std::string indexDataPath) :
    m_vertexDataPath(vertexDataPath), m_indexDataPath(indexDataPath)
{
    Read();
}

void VertexData::Init(std::string vertexDataPath, std::string indexDataPath)
{
    m_vertexDataPath = vertexDataPath;
    m_indexDataPath = indexDataPath;
    Read();
}

void VertexData::Read()
{
    std::vector<GLfloat>().swap(m_vertexData);
    std::vector<GLuint>().swap(m_indexData);

    ReadData(m_vertexDataPath, &VertexData::ParseVertexData);
    ReadData(m_indexDataPath, &VertexData::ParseIndexData);

    m_vertexData.shrink_to_fit();
    m_indexData.shrink_to_fit();
}

const std::vector<GLfloat>& VertexData::GetAttrData()
{
    return m_vertexData;
}

const std::vector<GLuint>& VertexData::GetIndexData()
{
    return m_indexData;
}

void VertexData::ReadData(std::string dataPath, CallBack cb)
{
    std::ifstream stream(dataPath, std::ios::in);
    if (!stream.is_open())
        return;

    std::string line;
    while (std::getline(stream, line)) {
        std::string::size_type pos = 0;
        // 判断是否是注释行
        pos = line.find_first_not_of(" \t\r\n", pos);
        if (pos == std::string::npos) // 当前行为空白行
            continue;

        if(line[pos] == '#') // 当前行:#开头的注释行
            continue;

        while (pos < line.size()) {
            auto sPos = line.find_first_not_of(",f \t\r\n", pos); // 找到首个非空白字符位置或非逗号和非字符'f'，因为浮点数字符串可能带有f，比如1.0f
            if (sPos == std::string::npos || line[sPos] == '#') { // 到了一行结尾，或者遇到了#注释符
                break;
            }

            auto ePos = line.find_first_of(",f \t\r\n", sPos);
            if (ePos == std::string::npos) { // 最后一个位置的数据
                (this->*cb)(line.substr(sPos));
                break;
            } else { // 正常位置的数据
                (this->*cb)(line.substr(sPos, ePos - sPos));
                pos = ePos + 1;
            }
        }
    }

    stream.close();
}

void VertexData::ParseVertexData(const std::string& str)
{
    m_vertexData.push_back(atof(str.c_str()));
}

void VertexData::ParseIndexData(const std::string& str)
{
    m_indexData.push_back(atoi(str.c_str()));
}