// author : zhoukang
// date   : 2022-08-26 23:46:13

#ifndef CG_HUAKE_WANLIN_DEMO1_VERTEXDATA_H
#define CG_HUAKE_WANLIN_DEMO1_VERTEXDATA_H

#include <vector>
#include <string>
#include "glad/glad.h"

/**
 * 顶点数据文件：顶点数据为浮点数，数据之间按照逗号隔开
 * 索引数据文件：索引数据为整数，数据之间按照逗号隔开
 *
 * 数据文件可以正确过滤注释和空白字符
 *  包括行首注释#、行尾注释#
 */
class VertexData {
private:
    std::string m_vertexDataPath;
    std::string m_indexDataPath;

    std::vector<GLfloat> m_vertexData;
    std::vector<GLuint> m_indexData;

public:
    VertexData(std::string vertexDataPath, std::string indexDataPath="");
    VertexData() {};

    void Init(std::string vertexDataPath, std::string indexDataPath="");

    const std::vector<GLfloat>& GetAttrData();

    const std::vector<GLuint>& GetIndexData();


private:
    void Read();

    typedef void (VertexData::*CallBack)(const std::string&);
    void ReadData(std::string dataPath, CallBack cb);
    void ParseVertexData(const std::string& str);
    void ParseIndexData(const std::string& str);
};


#endif //CG_HUAKE_WANLIN_DEMO1_VERTEXDATA_H
