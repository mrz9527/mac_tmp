//
// Created by xm210508 on 2021/7/12.
//
#include "../core/testBatch/TestBatch.h"
using namespace ChineseIme;
using namespace std;

vector<string> split(const string &str, const string &delim);
void DataProcessor(string input_path, string output_path);
int main(int argc, char *argv[]) {
    std::cout << "hello test batch" << std::endl;
    TestBatch testBatch;
    //zhuyin26 all (withTone or withoutTone or withPartTone) new dic data
    testBatch.TestBatchUniform("../../testcase/zhuyin26_all_newdic.txt","../../testcase/zhuyin26_all_newdic_out.txt", "../../testcase/sumTest.txt", "../../dict/zhuyin.dat", "../../dict/predict_nextword.dat", ChineseIme::KeyboardType::KBT_ZHUYIN_26);
    //zhuyin26 without tone raw dic data
    testBatch.TestBatchUniform("../../testcase/zhuyin26_withouttone_rawdic.txt","../../testcase/zhuyin26_withouttone_rawdic_out.txt", "../../testcase/sumTest.txt", "../../dict/zhuyin.dat", "../../dict/predict_nextword.dat", ChineseIme::KeyboardType::KBT_ZHUYIN_26);
    //zhuyin26 without tone new dic data
    testBatch.TestBatchUniform("../../testcase/zhuyin26_withouttone_newdic.txt","../../testcase/zhuyin26_withouttone_newdic_out.txt", "../../testcase/sumTest.txt", "../../dict/zhuyin.dat", "../../dict/predict_nextword.dat", ChineseIme::KeyboardType::KBT_ZHUYIN_26);
    //zhuyin26 with tone raw dic data
    testBatch.TestBatchUniform("../../testcase/zhuyin26_withtone_rawdic.txt","../../testcase/zhuyin26_withtone_rawdic_out.txt", "../../testcase/sumTest.txt", "../../dict/zhuyin.dat", "../../dict/predict_nextword.dat", ChineseIme::KeyboardType::KBT_ZHUYIN_26);
    //zhuyin26 with tone new dic data
    testBatch.TestBatchUniform("../../testcase/zhuyin26_withtone_newdic.txt","../../testcase/zhuyin26_withtone_newdic_out.txt", "../../testcase/sumTest.txt", "../../dict/zhuyin.dat", "../../dict/predict_nextword.dat", ChineseIme::KeyboardType::KBT_ZHUYIN_26);
    //zhuyin9 without tone raw dic data
    testBatch.TestBatchUniform("../../testcase/zhuyin9_rawdic.txt","../../testcase/zhuyin9_rawdic_out.txt", "../../testcase/sumTest.txt", "../../dict/zhuyin.dat", "../../dict/predict_nextword.dat", ChineseIme::KeyboardType::KBT_ZHUYIN_9);
    //zhuyin9 without tone new dic data
    testBatch.TestBatchUniform("../../testcase/zhuyin9_newdic.txt","../../testcase/zhuyin9_newdic_out.txt", "../../testcase/sumTest.txt", "../../dict/zhuyin.dat", "../../dict/predict_nextword.dat", ChineseIme::KeyboardType::KBT_ZHUYIN_9);
    //cangjie raw dic data
    testBatch.TestBatchUniform("../../testcase/cangjie_rawdic.txt", "../../testcase/cangjie_rawdic_out.txt", "../../testcase/sumTest.txt", "../../dict/cangjie.dat", "../../dict/predict_nextword.dat", ChineseIme::KeyboardType::KBT_CANGJIE);
    //cangjie fast new dic data
    testBatch.TestBatchUniform("../../testcase/cangjie_fast_newdic.txt", "../../testcase/cangjie_fast_newdic_out.txt", "../../testcase/sumTest.txt", "../../dict/cangjie.dat", "../../dict/predict_nextword.dat", ChineseIme::KeyboardType::KBT_CANGJIE_FAST);
    //cangjie new dic data
    testBatch.TestBatchUniform("../../testcase/cangjie_newdic.txt", "../../testcase/cangjie_newdic_out.txt", "../../testcase/sumTest.txt", "../../dict/cangjie.dat", "../../dict/predict_nextword.dat", ChineseIme::KeyboardType::KBT_CANGJIE);
    //pinyin9 raw dic data
    testBatch.TestBatchUniform("../../testcase/pinyin9_rawdic.txt", "../../testcase/pinyin9_rawdic_out.txt", "../../testcase/sumTest.txt", "../../dict/pinyin.dat", "../../dict/predict_nextword.dat", ChineseIme::KeyboardType::KBT_PINYIN_9);
    //pinyin9 new dic data
    testBatch.TestBatchUniform("../../testcase/pinyin9_newdic.txt", "../../testcase/pinyin9_newdic_out.txt", "../../testcase/sumTest.txt", "../../dict/pinyin.dat", "../../dict/predict_nextword.dat", ChineseIme::KeyboardType::KBT_PINYIN_9);
    //pinyin9 testcaseV1 2000+
    testBatch.TestBatchUniform("../../testcase/pinyin9_testcaseV1.txt", "../../testcase/pinyin9_testcaseV1_out.txt", "../../testcase/sumTest.txt", "../../dict/pinyin.dat", "../../dict/predict_nextword.dat", ChineseIme::KeyboardType::KBT_PINYIN_9);
    //pinyin26 raw dic data
    testBatch.TestBatchUniform("../../testcase/pinyin26_rawdic.txt", "../../testcase/pinyin26_rawdic_out.txt", "../../testcase/sumTest.txt", "../../dict/pinyin.dat", "../../dict/predict_nextword.dat", ChineseIme::KeyboardType::KBT_PINYIN_26);
    //pinyin26 new dic data
    testBatch.TestBatchUniform("../../testcase/pinyin26_newdic.txt", "../../testcase/pinyin26_newdic_out.txt", "../../testcase/sumTest.txt", "../../dict/pinyin.dat", "../../dict/predict_nextword.dat", ChineseIme::KeyboardType::KBT_PINYIN_26);
    //pinyin26 testcaseV1 2000+
    testBatch.TestBatchUniform("../../testcase/pinyin26_testcaseV1.txt", "../../testcase/pinyin26_testcaseV1_out.txt", "../../testcase/sumTest.txt", "../../dict/pinyin.dat", "../../dict/predict_nextword.dat", ChineseIme::KeyboardType::KBT_PINYIN_26);
    //bihua new dic data
    testBatch.TestBatchUniform("../../testcase/bihua_newdic.txt", "../../testcase/bihua_newdic_out.txt", "../../testcase/sumTest.txt", "../../dict/bihua.dat", "../../dict/predict_nextword.dat", ChineseIme::KeyboardType::KBT_BIHUA_FANTI);
    //bihua raw dic data
    testBatch.TestBatchUniform("../../testcase/bihua_rawdic.txt", "../../testcase/bihua_rawdic_out.txt", "../../testcase/sumTest.txt", "../../dict/bihua.dat", "../../dict/predict_nextword.dat", ChineseIme::KeyboardType::KBT_BIHUA_FANTI);
    //next word jianti new dic data
    testBatch.TestBatchUniform("../../testcase/nextword_newdic.txt", "../../testcase/nextword_newdic_out.txt", "../../testcase/sumTest.txt", "../../dict/pinyin.dat", "../../dict/predict_nextword.dat", ChineseIme::KeyboardType::KBT_UNKNOWN);
    //next word fanti new dic data
    testBatch.TestBatchUniform("../../testcase/nextword_trad_newdic.txt", "../../testcase/nextword_trad_newdic_out.txt", "../../testcase/sumTest.txt", "../../dict/pinyin.dat", "../../dict/predict_nextword_trad.dat", ChineseIme::KeyboardType::KBT_UNKNOWN);
    //next word testcase 2w
    testBatch.TestBatchUniform("../../testcase/predict_2w.txt", "../../testcase/predict_2w_out.txt", "../../testcase/sumTest.txt", "../../dict/pinyin.dat", "../../dict/predict_nextword.dat", ChineseIme::KeyboardType::KBT_UNKNOWN);

    //data processing
    //DataProcessor("../../testcase/zhuyin26_newdic_needprosess.txt", "../../testcase/zhuyin26_all_newdic.txt");

    return 0;
}

void DataProcessor(string input_path, string output_path) {
    string line;
    FILE * fp;
    ifstream in(input_path);

    fp = fopen (output_path.c_str(), "w");
    while (getline(in, line)) {
        vector<string> vec_line = split(line, "\t");
        vector<string> vec_word = split(vec_line[0], " ");
        int count = (vec_line.size() - 2) / 2;
        int wordCount = vec_word.size();
        for(int i = 0; i < count; i++) {
//            fprintf(fp, "%s ", vec_line[i*2+2].substr(0, wordCount*3).c_str());
            int jcount = 1;
            for(int t = 0; t < wordCount; t++) {
                jcount = jcount*2;
            }
            for(int j = 0; j < jcount; j++) {
                string result = "";
                for(int m = wordCount; m > 0; m--) {
                    int mcount = 1;
                    for(int t = 0; t < m; t++) {
                        mcount = mcount*2;
                    }
                    int isTone = (j%mcount)/(mcount/2);
                    if(isTone == 1) {
                        result = result + vec_word[wordCount - m] + vec_line[i*2+2].substr(wordCount*3 + (wordCount - m), 1);
                    } else if(isTone == 0) {
                        result = result + vec_word[wordCount - m];
                    }
                }
                fprintf(fp, "%s %s\n", vec_line[i*2+2].substr(0, wordCount*3).c_str(), result.c_str());
            }
//            fprintf(fp, "\n");
        }
//        fprintf(fp, "%s %s\n", vec_line[0].substr(0, vec_line[0].length() - 3).c_str(), vec_line[0].substr(vec_line[0].length() - 3, 3).c_str());
    }
    fclose(fp);
}

vector<string> split(const string &str, const string &delim) {
    vector<string> res;
    if ("" == str) return res;
    //先将要切割的字符串从string类型转换为char*类型
    char *strs = new char[str.length() + 1]; //不要忘了
    strcpy(strs, str.c_str());

    char *d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while (p) {
        string s = p; //分割得到的字符串转换为string类型
        res.push_back(s); //存入结果数组
        p = strtok(NULL, d);
    }

    return res;
}
