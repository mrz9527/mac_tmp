// author : zhoukang
// date   : 2022-08-26 21:05:12

#include "Shader.h"

Shader::Shader(GLenum shaderType, std::string source, bool isSourceFile) :
    m_type(shaderType), m_source(source), m_isSourceFile(isSourceFile)
{
    m_shader = glCreateShader(shaderType);
}

bool Shader::Compile()
{
    std::string strSource = ReadSourceFile();
    const char* sSource = strSource.c_str();
    glShaderSource(m_shader, 1, &sSource, nullptr);

    glCompileShader(m_shader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(m_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }
    return true;
}

GLuint Shader::Get()
{
    return m_shader;
}

std::string Shader::ReadSourceFile()
{
    if (!m_isSourceFile) return m_source;

    std::ifstream stream(m_source, std::ios::in);
    if (!stream.is_open()) {
        return "";
    }

    std::stringstream strStream;
    // 读取文件的缓冲内容到流中
    strStream << stream.rdbuf();
    // 关闭文件
    stream.close();

    std::string strSource = strStream.str();
    return strSource;
}

ShaderProgram::ShaderProgram(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath)
{
    Shader vertexShader(GL_VERTEX_SHADER, vertexShaderPath, true);
    Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderPath, true);
    vertexShader.Compile();
    fragmentShader.Compile();

    m_program = glCreateProgram();
    glAttachShader(m_program, vertexShader.Get());
    glAttachShader(m_program, fragmentShader.Get());

    glLinkProgram(m_program);

    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader.Get());
    glDeleteShader(fragmentShader.Get());
}

// 使用程序
void ShaderProgram::Use()
{
    glUseProgram(m_program);
}

GLuint ShaderProgram::Get()
{
    return m_program;
}