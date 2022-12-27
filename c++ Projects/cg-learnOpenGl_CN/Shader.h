// author : zhoukang
// date   : 2022-08-26 21:05:12

#ifndef CG_HUAKE_WANLIN_DEMO1_SHADER_H
#define CG_HUAKE_WANLIN_DEMO1_SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glad/glad.h"

#include <string>
class Shader{
private:
    GLenum m_type;
    std::string m_source;
    bool m_isSourceFile;

    GLuint m_shader;
public:
    // 说明：顶点着色器源码中，gl_Position = vec4(position, 1.0f)，第四个分量设置为1.0f，不能为0.0f，否则不显示
    Shader(GLenum shaderType, std::string source, bool isSourceFile);
    bool Compile();
    GLuint Get();

private:
    std::string ReadSourceFile();
};

class ShaderProgram
{
private:
    GLuint m_program;
public:
    // 构造器读取并构建着色器
    ShaderProgram(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath);

    // 使用程序
    void Use();

    GLuint Get();
};

#endif //CG_HUAKE_WANLIN_DEMO1_SHADER_H
