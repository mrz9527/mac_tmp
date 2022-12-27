#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <soil/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include "cmath"
#include "Shader.h"
#include "VertexData.h"



// 设置顶点属性指针，用来解释顶点
// 0: 属性位置索引。
//      位置变量的属性位置索引一般设置为0（顶点着色器程序中layout(location=0) in vec3 position
//      颜色变量的属性位置索引一般设置为1（顶点着色器程序中layout(location=1) in vec3 color。如果顶点数据中包含颜色数据的话。
//      当然，顶点着色器程序中layout(location)可以是其他值，只要glVertexAttribPointer的第一个参数与layout(location)保持一致就行。
// 3: 每个顶点使用3个分量
// GL_FLOAT: 每个分量的数据类型
// GL_FALSE: 不适用规范化设备坐标
// 3 * sizeof(float)：每个顶点的步长，一个顶点3个分量，每个分量长度为sizeof(float)
// (void*)0：偏移位置。
//      一般，第一个顶点的位置数据偏移为0。
//      第一个顶点的颜色数据偏移为3 * sizeof(float)。
//      因为前三个是顶点的位置数据分量，顶点的颜色数据分量紧跟在位置数据之后。
//glVertexAttribPointer(0, vertex_dim, GL_FLOAT, GL_FALSE, vertex_dim * sizeof(GLfloat), (void*)0);

GLfloat mixValue_6 = 0.2f;
GLFWwindow* Init(int win_width = 800, int win_heiht = 600)
{
    // 1.初始化工作
    // 1.1 初始化GLFW
    //glfwInit();
    if (!glfwInit()) {
        std::cout << "Failed to Init" << std::endl;
        return nullptr;
    }

    int Major, Minor, Rev;
    glfwGetVersion(&Major, &Minor, &Rev);
    printf("GLFW %d.%d.%d initialized\n", Major, Minor, Rev);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL3.3,主版本号为3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL3.3,次版本号为3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 使用核心模式（无须向后兼容）

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 针对MacOS X
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // 设置窗口大小不可改变

    std::cout << "Success to initialize GLFW" << std::endl;

    // 1.2 创建窗口
    GLFWwindow* window = glfwCreateWindow(win_width, win_heiht, "Computer Graphics", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to Create OpenGL Contex" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    std::cout << "Success to Create OpenGL Contex" << std::endl;

    glfwMakeContextCurrent(window); // 将窗口的上下文设置为当前线程的主上下文

    // 1.3 初始化GLAD
    if (gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) == 0) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }
    std::cout << "Success to initialize GLAD" << std::endl;

    // 1.4 创建视口
    int view_width, view_height;
    glfwGetFramebufferSize(window, &view_width, &view_height);
    glViewport(0, 0, view_width, view_height); // 前两个参数指定左下角位置，后两个参数是渲染窗口的宽、高

    return window;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void key_callback_6(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        mixValue_6 += 0.1f;
        if(mixValue_6 >= 1.0f)
            mixValue_6 = 1.0f;
    } else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        mixValue_6 -= 0.1f;
        if(mixValue_6 <= 0.0f)
            mixValue_6 = 0.0f;
    }
}

// ---------------------------------读取顶点属性数据-----------------------------------------------------
void ReadVertexAttr_1(std::vector<VertexData>& vertexDatas)
{
    VertexData vertexData("../res/vertex.attr/1_triangle/pos.va");
    vertexDatas.push_back(vertexData);
}

void ReadVertexAttr_2(std::vector<VertexData>& vertexDatas)
{
    VertexData vertexData2("../res/vertex.attr/2_rectangle/pos.va",
                           "../res/vertex.attr/2_rectangle/indices.va");
    vertexDatas.push_back(vertexData2);
}

void ReadVertexAttr_3(std::vector<VertexData>& vertexDatas)
{
    VertexData vertexData3("../res/vertex.attr/3_triangle/pos_color.va");
    vertexDatas.push_back(vertexData3);
}

void ReadVertexAttr_4(std::vector<VertexData>& vertexDatas)
{
    VertexData vertexData4("../res/vertex.attr/4_triangle/pos_color.va",
                           "../res/vertex.attr/4_triangle/indices.va");
    vertexDatas.push_back(vertexData4);
}

void ReadVertexAttr_5(VertexData& vertexData)
{
    vertexData.Init("../res/vertex.attr/5_rectangle/pos_color_texture.va",
                    "../res/vertex.attr/5_rectangle/indices.va");
}

void ReadVertexAttr_6(VertexData& vertexData)
{
    vertexData.Init("../res/vertex.attr/6_rectangle/pos_color_texture.va",
                    "../res/vertex.attr/6_rectangle/indices.va");
}

void ReadVertexAttr_7(VertexData& vertexData)
{
    vertexData.Init("../res/vertex.attr/7_rectangle/pos_color_texture.va",
                    "../res/vertex.attr/7_rectangle/indices.va");
}

void ReadVertexAttr_8(VertexData& vertexData)
{
    vertexData.Init("../res/vertex.attr/8_rectangle/pos_color_texture.va",
                    "../res/vertex.attr/8_rectangle/indices.va");
}

// ---------------------------------配置和解析顶点缓冲对象-----------------------------------------------------
// 包括vao、vbo、ebo创建、vao、vbo、ebo绑定、vbo、ebo数据拷贝、设置顶点属性、解绑。
template<GLint vertex_dim = 3>
void ConfigVertexAttr_1(std::vector<GLuint>& vaos, std::vector<GLuint>& vbos,
                        std::vector<VertexData> vertexDatas)
{
    GLsizei vboNum = vertexDatas.size();
    glGenVertexArrays(vboNum, vaos.data());
    glGenBuffers(vboNum, vbos.data());

    for (int i = 0; i < vboNum; ++i) {
        // 绑定对象。
        // 一次只能绑定一个对象，具体流程如下：
        // 绑定一个vao、vbo，然后数据拷贝，然后设置顶点属性，解绑。之后才能绑定另一个vao、vbo
        glBindVertexArray(vaos[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);

        // 数据拷贝
        auto vertexData = vertexDatas[i].GetAttrData();
        int vertexNum = vertexData.size();
        glBufferData(GL_ARRAY_BUFFER, vertexNum * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

        // 设置顶点属性指针，用来解释顶点
        glVertexAttribPointer(0, vertex_dim, GL_FLOAT, GL_FALSE, vertex_dim * sizeof(GLfloat), (void*) 0);
        glEnableVertexAttribArray(0);
        // 解绑
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0); // vao最后解绑
    }
}

/**
 *
 * @tparam vertex_dim 模板参数，定义了一个顶点的分量数，默认是三个分量，代表xyz
 * @param vaos "顶点数组对象"数组，数组长度为vboNums
 * @param vbos "顶点缓冲对象"数组，数组长度为vboNums
 * @param ebos "索引缓冲对象"数组，数组长度为vboNums
 * @param vertexss  顶点数据数组，一组顶点数据，就是一个vertexss[i]
 * @param indexss   索引数据数组，一组索引数据，就是一个indexss[i]，是对一组顶点数据vertexss[i]的索引
 *
 * 说明： 一般一个图形，使用一组顶点数据，一组顶点数组使用一个顶点缓冲对象VBO，一个顶点缓冲对象VBO配套使用一个顶点数组对象BAO
 *      在使用索引缓冲对象的情况下，一个顶点缓冲对象VBO对应一个索引缓冲对象EBO
 */
template<GLint vertex_dim = 3>
void ConfigVertexAttr_2(std::vector<GLuint>& vaos, std::vector<GLuint>& vbos, std::vector<GLuint>& ebos,
                        std::vector<VertexData> vertexDatas)
{
    GLsizei vboNum = vertexDatas.size();
    glGenVertexArrays(vboNum, vaos.data());
    glGenBuffers(vboNum, vbos.data());
    glGenBuffers(vboNum, ebos.data());

    for (int i = 0; i < vboNum; ++i) {
        // 一次只能绑定一个对象，具体流程如下：
        // 绑定一个vao、vbo,veo，然后数据拷贝，然后设置顶点属性，解绑。之后才能绑定另一个vao、vbo、ebo
        glBindVertexArray(vaos[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[i]);

        // 顶点数据拷贝
        auto vertexData = vertexDatas[i].GetAttrData();
        int vertexNum = vertexData.size();
        glBufferData(GL_ARRAY_BUFFER, vertexNum * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

        // 索引数据拷贝
        auto indexData = vertexDatas[i].GetIndexData();
        int indexNum = indexData.size();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexNum * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW);

        // 设置顶点属性指针，用来解释顶点
        glVertexAttribPointer(0, vertex_dim, GL_FLOAT, GL_FALSE, vertex_dim * sizeof(GLfloat), (void*) 0);
        glEnableVertexAttribArray(0);
        // 解绑
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0); // vao最后解绑
    }
}

template<GLint vertex_dim = 3>
void ConfigVertexAttr_3(std::vector<GLuint>& vaos, std::vector<GLuint>& vbos,
                        std::vector<VertexData> vertexDatas)
{
    GLsizei vboNum = vertexDatas.size();
    glGenVertexArrays(vboNum, vaos.data());
    glGenBuffers(vboNum, vbos.data());

    for (int i = 0; i < vboNum; ++i) {
        // 绑定对象。
        // 一次只能绑定一个对象，具体流程如下：
        // 绑定一个vao、vbo，然后数据拷贝，然后设置顶点属性，解绑。之后才能绑定另一个vao、vbo
        glBindVertexArray(vaos[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);

        // 数据拷贝
        auto vertexData = vertexDatas[i].GetAttrData();
        int vertexNum = vertexData.size();
        glBufferData(GL_ARRAY_BUFFER, vertexNum * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

        // 设置顶点属性指针，用来解释顶点
        // glVertexAttribPointer的第一个参数表示索引，与顶点着色器源码中的layout(location = 0)保持一致，用于解释坐标分量。
        // 第一个参数还可以通过glGetAttribLocation(shader_program, "position")来获取位置属性索引
        // glGetAttribLocation(shader_program, "color")来获取颜色属性索引
        glVertexAttribPointer(0, vertex_dim, GL_FLOAT, GL_FALSE, 2 * vertex_dim * sizeof(GLfloat), (void*) 0);
        glEnableVertexAttribArray(0);
        // glVertexAttribPointer的第一个参数表示索引，与顶点着色器源码中的layout(location = 1)保持一致，用于解释颜色分量。
        glVertexAttribPointer(1, vertex_dim, GL_FLOAT, GL_FALSE, 2 * vertex_dim * sizeof(GLfloat),
                              (void*) (vertex_dim * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // 解绑
        glBindVertexArray(0); // vao最后解绑
    }
}

template<GLint vertex_dim = 3>
void ConfigVertexAttr_4(std::vector<GLuint>& vaos, std::vector<GLuint>& vbos, std::vector<GLuint>& ebos,
                        std::vector<VertexData> vertexDatas)
{
    GLsizei vboNum = vertexDatas.size();
    glGenVertexArrays(vboNum, vaos.data());
    glGenBuffers(vboNum, vbos.data());
    glGenBuffers(vboNum, ebos.data());

    for (int i = 0; i < vboNum; ++i) {
        // 一次只能绑定一个对象，具体流程如下：
        // 绑定一个vao、vbo,veo，然后数据拷贝，然后设置顶点属性，解绑。之后才能绑定另一个vao、vbo、ebo
        glBindVertexArray(vaos[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[i]);

        // 顶点数据拷贝
        auto vertexData = vertexDatas[i].GetAttrData();
        int vertexNum = vertexData.size();
        glBufferData(GL_ARRAY_BUFFER, vertexNum * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

        // 索引数据拷贝
        auto indexData = vertexDatas[i].GetIndexData();
        int indexNum = indexData.size();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexNum * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW);

        // 设置顶点属性指针，用来解释顶点
        glVertexAttribPointer(0, vertex_dim, GL_FLOAT, GL_FALSE, 2 * vertex_dim * sizeof(GLfloat), (void*) 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, vertex_dim, GL_FLOAT, GL_FALSE, 2 * vertex_dim * sizeof(GLfloat),
                              (void*) (vertex_dim * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        // 解绑
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0); // vao最后解绑
    }
}

template<GLint vertex_dim = 3>
void ConfigVertexAttr_5(GLuint& vao, GLuint& vbo, GLuint& ebo, VertexData vertexData)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    // 顶点属性拷贝
    auto attrData = vertexData.GetAttrData();
    int vertexNum = attrData.size();
    glBufferData(GL_ARRAY_BUFFER, vertexNum * sizeof(GLfloat), attrData.data(), GL_STATIC_DRAW);

    // 索引数据拷贝
    auto indexData = vertexData.GetIndexData();
    int indexNum = indexData.size();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexNum * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW);

    // 设置顶点属性指针，用来解释顶点
    int step = 2 * vertex_dim + 2; // pos坐标3个分量，color坐标3个分量，纹理坐标2个分量
    int floatSize = sizeof(GLfloat);
    // 位置属性 pos
    glVertexAttribPointer(0, vertex_dim, GL_FLOAT, GL_FALSE, step * floatSize, (void*) 0);
    glEnableVertexAttribArray(0);
    // 颜色属性 color
    glVertexAttribPointer(1, vertex_dim, GL_FLOAT, GL_FALSE, step * floatSize, (void*) (vertex_dim * floatSize));
    glEnableVertexAttribArray(1);
    // 纹理属性 texture 2D纹理坐标只有两个维度
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, step * floatSize, (void*) (2 * vertex_dim * floatSize));
    glEnableVertexAttribArray(2);

    // 解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // vao最后解绑
}


#define ConfigVertexAttr_6 ConfigVertexAttr_5
#define ConfigVertexAttr_7 ConfigVertexAttr_5

// ---------------------------------实时渲染-----------------------------------------------------
void RenderTriangle_1(GLFWwindow* window, std::vector<ShaderProgram> shaderPrograms, std::vector<GLuint> vaos,
                      std::vector<VertexData> vertexDatas)
{
    int vboNum = vertexDatas.size();
    while (!glfwWindowShouldClose(window)) {
        /*
            while渲染过程
                清除图像背景 glClearColor()
                清除缓冲 glClear()

                使用着色器程序 glUseProgram(shaderProgram)
                绑定VAO   glBindVertexArray(VAO)
                绘制图形   glDrawArrays/glDrawElements 说明：每次绘制之前，都要使用着色器程序，绑定VAO;绘制之后，都要解绑VAO
                解绑VAO   glBindVertexArray(0)

                检测事件    glfwPollEvents
                交互缓冲    glfwSwapBuffers
            }

         * */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i < vboNum; ++i) {
            shaderPrograms[i].Use();
            //glUseProgram(shaderPrograms[i]);
            glBindVertexArray(vaos[i]);
            glDrawArrays(GL_TRIANGLES, 0, vertexDatas[i].GetAttrData().size()); // 基于vao中的vbo绘制
            //glDrawElements(GL_TRIANGLES, vertexss[i].size(), GL_UNSIGNED_INT, 0); // 基于vao中的ebo绘制
            glBindVertexArray(0);
        }

        // 检测事件，如果有事件发生，执行相应的事件回调
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

/** 使用ebo方式渲染，每次渲染时，更新片段着色器的颜色
 *
 * @param window
 * @param shaderPrograms
 * @param vaos
 * @param vertexDatas
 */
void RenderRectangle_2(GLFWwindow* window, std::vector<ShaderProgram> shaderPrograms, std::vector<GLuint> vaos,
                       std::vector<VertexData> vertexDatas)
{
    int vboNum = vertexDatas.size();
    while (!glfwWindowShouldClose(window)) {
        /*
            while渲染过程
                清除图像背景 glClearColor()
                清除缓冲 glClear()

                使用着色器程序 glUseProgram(shaderProgram)
                绑定VAO   glBindVertexArray(VAO)
                绘制图形   glDrawArrays/glDrawElements 说明：每次绘制之前，都要使用着色器程序，绑定VAO;绘制之后，都要解绑VAO
                解绑VAO   glBindVertexArray(0)

                检测事件    glfwPollEvents
                交互缓冲    glfwSwapBuffers
            }

         * */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        double time = glfwGetTime();
        GLint uniformColorPos = glGetUniformLocation(shaderPrograms[0].Get(), "uniformColor");
        GLfloat redValue = sin(time) / 2 + 0.5f;
        glUniform4f(uniformColorPos, redValue, 0.0f, 0.0f, 1.0f);

        for (int i = 0; i < vboNum; ++i) {
            shaderPrograms[i].Use();
            //glUseProgram(shaderPrograms[i]);
            glBindVertexArray(vaos[i]);
            glDrawElements(GL_TRIANGLES, vertexDatas[i].GetIndexData().size(), GL_UNSIGNED_INT, 0); // 基于vao中的ebo绘制
            glBindVertexArray(0);
        }

        // 检测事件，如果有事件发生，执行相应的事件回调
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void RenderTriangle_3(GLFWwindow* window, std::vector<ShaderProgram> shaderPrograms,
                      std::vector<GLuint> vaos,
                      std::vector<VertexData> vertexDatas)
{
    int vboNum = vertexDatas.size();
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i < vboNum; ++i) {
            shaderPrograms[i].Use();
            glBindVertexArray(vaos[i]);
            glDrawArrays(GL_TRIANGLES, 0, vertexDatas[i].GetAttrData().size() / 2 / 3); // 基于vao中的vbo绘制
            glBindVertexArray(0);
        }

        // 检测事件，如果有事件发生，执行相应的事件回调
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void RenderTriangle_4(GLFWwindow* window, std::vector<ShaderProgram> shaderPrograms,
                      std::vector<GLuint> vaos,
                      std::vector<VertexData> vertexDatas)
{
    int vboNum = vertexDatas.size();
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i < vboNum; ++i) {
            shaderPrograms[i].Use();
            glBindVertexArray(vaos[i]);
            //glDrawElements(GL_TRIANGLES, vertexDatas[i].GetIndexData().size() / 2 * 3, GL_UNSIGNED_INT, 0); // 基于vao中的ebo绘制
            glDrawElements(GL_TRIANGLES, vertexDatas[i].GetIndexData().size(), GL_UNSIGNED_INT, 0); // 基于vao中的ebo绘制
            glBindVertexArray(0);
        }

        // 检测事件，如果有事件发生，执行相应的事件回调
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void RenderRectangle_5(GLFWwindow* window, ShaderProgram shaderProgram, const GLuint& vao, const GLuint& texture,
                       VertexData vertexData)
{
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.Use();
        // 绑定顶点位置属性和顶点纹理属性
        glBindVertexArray(vao); // vao属于顶点位置属性pos，关联了vbo和ebo，没有关联顶点纹理属性texture，所以顶点纹理属性要单独绑定
        glBindTexture(GL_TEXTURE_2D, texture); // 绑定纹理
        glDrawElements(GL_TRIANGLES, vertexData.GetIndexData().size(), GL_UNSIGNED_INT, 0); // 基于vao中的ebo绘制

        glBindVertexArray(0);

        // 检测事件，如果有事件发生，执行相应的事件回调
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void RenderRectangle_6(GLFWwindow* window, ShaderProgram shaderProgram, const GLuint& vao,
                       const std::vector<GLuint>& textures,
                       VertexData vertexData)
{
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.Use();
        // 绑定顶点位置属性和顶点纹理属性

        for (int i = 0; i < textures.size(); ++i) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textures[i]);
            char textureName[20] = {0};
            sprintf(textureName, "ourTexture%d", i + 1);
            glUniform1i(glGetUniformLocation(shaderProgram.Get(), textureName), i);
        }

        glUniform1f(glGetUniformLocation(shaderProgram.Get(), "mixValue"), mixValue_6);

        glBindVertexArray(vao); // vao属于顶点位置属性pos，关联了vbo和ebo，没有关联顶点纹理属性texture，所以顶点纹理属性要单独绑定
        glDrawElements(GL_TRIANGLES, vertexData.GetIndexData().size(), GL_UNSIGNED_INT, 0); // 基于vao中的ebo绘制

        glBindVertexArray(0);

        // 检测事件，如果有事件发生，执行相应的事件回调
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

// ---------------------------------纹理对象生成和绑定-----------------------------------------------------
void ReadAndConfigTexture_5(GLuint& texture)
{
    int width, height;
    unsigned char* imageBuf = SOIL_load_image("../res/image/container.jpeg", &width, &height, 0, SOIL_LOAD_RGB);
    if (!imageBuf)
        return;

    // 生成纹理对象并绑定
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // 纹理对象关联纹理图像
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageBuf);
    // 根据纹理对象，生成贴图mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(imageBuf);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ReadAndConfigTexture_6(std::vector<GLuint>& textures)
{
    std::vector<std::string> images = {"../res/image/container.jpeg", "../res/image/awesomeface.png"};
    int imageCount = images.size();

    // 生成纹理对象
    textures.resize(imageCount);
    glGenTextures(imageCount, textures.data());

    for (int i = 0; i < imageCount; ++i) {
        int width, height;
        unsigned char* imageBuf = SOIL_load_image(images[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
        if (!imageBuf) {
            printf("读取失败 : %s\n", images[i].c_str());
            return;
        }

        glBindTexture(GL_TEXTURE_2D, textures[i]);

        // 纹理对象关联纹理图像
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageBuf);
        // 根据纹理对象，生成贴图mipmap
        glGenerateMipmap(GL_TEXTURE_2D);

        SOIL_free_image_data(imageBuf);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void ReadAndConfigTexture_7(std::vector<GLuint>& textures)
{
    std::vector<std::string> images = {"../res/image/container.jpeg", "../res/image/awesomeface.png"};
    int imageCount = images.size();

    // 生成纹理对象
    textures.resize(imageCount);
    glGenTextures(imageCount, textures.data());

    for (int i = 0; i < imageCount; ++i) {
        int width, height;
        unsigned char* imageBuf = SOIL_load_image(images[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
        if (!imageBuf) {
            printf("读取失败 : %s\n", images[i].c_str());
            return;
        }

        glBindTexture(GL_TEXTURE_2D, textures[i]);
        if (i == 1) { // 将纹理2环绕方式设置为镜像重复，纹理1默认为重复
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        }

        // 纹理对象关联纹理图像
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageBuf);
        // 根据纹理对象，生成贴图mipmap
        glGenerateMipmap(GL_TEXTURE_2D);

        SOIL_free_image_data(imageBuf);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void ReadAndConfigTexture_8(std::vector<GLuint>& textures)
{
    std::vector<std::string> images = {"../res/image/container.jpeg", "../res/image/awesomeface.png"};
    int imageCount = images.size();

    // 生成纹理对象
    textures.resize(imageCount);
    glGenTextures(imageCount, textures.data());

    for (int i = 0; i < imageCount; ++i) {
        int width, height;
        unsigned char* imageBuf = SOIL_load_image(images[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
        if (!imageBuf) {
            printf("读取失败 : %s\n", images[i].c_str());
            return;
        }

        glBindTexture(GL_TEXTURE_2D, textures[i]);

        // 当纹理坐标超出范围[0,1]时，设置环绕方式才有效。
        // 当纹理图像和图形尺寸不匹配，此时纹理图像会进行缩放，此时设置纹理过滤方式有效（linear和nearest)
        if (i == 1) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        // 纹理对象关联纹理图像
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageBuf);
        // 根据纹理对象，生成贴图mipmap
        glGenerateMipmap(GL_TEXTURE_2D);

        SOIL_free_image_data(imageBuf);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

// 只使用了vao和vbo
void DrawTriangle_1()
{
    printf("\nDrawTriangle_1 : 绘制三角形。\n"
           "\t技术点:\n"
           "\t\t采用位置属性(vao + vbo):在顶点属性文件中设置;\n"
           "\t\t线框模式\n"
           "\t\t渲染: 采用vao中的vbo来渲染,glDrawArrays\n");
    // 1.初始化工作：
    GLFWwindow* window = Init();

    // 注册按键回调
    // 回调注册时机：在窗口创建之后，渲染之前，都可以添加回调。
    glfwSetKeyCallback(window, key_callback);

    // 2.着色器程序(封装了顶点着色器和片段着色器)
    ShaderProgram shaderProgram("../res/shader/1_triangle/vertex.glsl",
                                "../res/shader/1_triangle/fragment.glsl");

    // 配置顶点数据
    std::vector<VertexData> vertexDatas;
    ReadVertexAttr_1(vertexDatas);

    // 顶点对象的封装。包括vao、vbo、ebo创建、vao、vbo、ebo绑定、vbo、ebo数据拷贝、设置顶点属性、解绑。
    int vboNum = vertexDatas.size();
    std::vector<GLuint> vaos(vboNum), vbos(vboNum);
    ConfigVertexAttr_1(vaos, vbos, vertexDatas);

    // 为了通用，为每一个顶点缓冲对象，配置一个着色器程序
    // 这里只用一个着色器程序，如果有多个着色器程序，可以配置多个
    std::vector<ShaderProgram> shaderPrograms(vboNum, shaderProgram);

    // 线框模式
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    RenderTriangle_1(window, shaderPrograms, vaos, vertexDatas);

    // 9 删除VAO和VBO
    glDeleteVertexArrays(vboNum, vaos.data());
    glDeleteBuffers(vboNum, vbos.data());

    // 10 清理所有资源并正确退出程序
    glfwTerminate();
}

// 使用了vao、vbo和veo
int DrawRectangle_2()
{
    printf("\nDrawRectangle_2 : 绘制四边形。\n"
           "\t技术点:\n"
           "\t\t采用位置属性(vao + vbo + ebo):在顶点属性文件中设置;\n"
           "\t\t采用颜色属性(uniform方式设置颜色属性):在着色器文件中定义uniform变量，在程序中设置颜色的值,glUniform4f;\n"
           "\t\t线框模式\n"
           "\t\t渲染: 采用vao中的ebo来渲染,glDrawElements\n");

    // 初始化工作
    GLFWwindow* window = Init();
    if (window == nullptr) return -1;

    // 注册按键回调
    // 回调注册时机：在窗口创建之后，渲染之前，都可以添加回调。
    glfwSetKeyCallback(window, key_callback);

    ShaderProgram shaderProgram("../res/shader/2_rectangle/vertex.glsl",
                                "../res/shader/2_rectangle/fragment.glsl");

    // 从文件中读取顶点属性数据
    std::vector<VertexData> vertexDatas;
    ReadVertexAttr_2(vertexDatas);
    const int vboNum = vertexDatas.size();

    // 顶点对象的封装。包括vao、vbo、ebo创建、vao、vbo、ebo绑定、vbo、ebo数据拷贝、设置顶点属性、解绑。
    std::vector<GLuint> vaos(vboNum), vbos(vboNum), ebos(vboNum);
    ConfigVertexAttr_2(vaos, vbos, ebos, vertexDatas);

    // 为了通用，为每一个顶点缓冲对象，配置一个着色器程序
    // 这里只用一个着色器程序，如果有多个着色器程序，可以配置多个
    std::vector<ShaderProgram> shaderPrograms(vboNum, shaderProgram);

    // 线框模式
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 渲染处理
    RenderRectangle_2(window, shaderPrograms, vaos, vertexDatas);

    // 删除VAO, VBO, EBO，释放资源
    glDeleteVertexArrays(vboNum, vaos.data());
    glDeleteBuffers(vboNum, vbos.data());
    glDeleteBuffers(vboNum, ebos.data());

    // 终止绘制，释放所有资源
    glfwTerminate();
    return 0;
}

// 只使用了vao和vbo
void DrawTriangle_3()
{
    printf("\nDrawTriangle_3 : 绘制三角形。\n"
           "\t技术点:\n"
           "\t\t采用位置属性(vao + vbo):在顶点属性文件中设置;\n"
           "\t\t采用颜色属性:在顶点属性中为顶点指定颜色属性，在顶点着色器中读入颜色属性，传递给片段着色器;\n"
           "\t\t线框模式\n"
           "\t\t渲染: 采用vao中的vbo来渲染,glDrawArrays\n");

    // 1.初始化工作：
    GLFWwindow* window = Init();

    // 注册按键回调
    // 回调注册时机：在窗口创建之后，渲染之前，都可以添加回调。
    glfwSetKeyCallback(window, key_callback);

    // 2.着色器程序(封装了顶点着色器和片段着色器)
    ShaderProgram shaderProgram("../res/shader/3_triangle/vertex.glsl",
                                "../res/shader/3_triangle/fragment.glsl");

    // 配置顶点数据
    std::vector<VertexData> vertexDatas;
    ReadVertexAttr_3(vertexDatas);

    // 顶点对象的封装。包括vao、vbo、ebo创建、vao、vbo、ebo绑定、vbo、ebo数据拷贝、设置顶点属性、解绑。
    int vboNum = vertexDatas.size();
    std::vector<GLuint> vaos(vboNum), vbos(vboNum);
    ConfigVertexAttr_3(vaos, vbos, vertexDatas);

    // 为了通用，为每一个顶点缓冲对象，配置一个着色器程序
    // 这里只用一个着色器程序，如果有多个着色器程序，可以配置多个
    std::vector<ShaderProgram> shaderPrograms(vboNum, shaderProgram);

    // 线框模式
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    RenderTriangle_3(window, shaderPrograms, vaos, vertexDatas);

    // 9 删除VAO和VBO
    glDeleteVertexArrays(vboNum, vaos.data());
    glDeleteBuffers(vboNum, vbos.data());

    // 10 清理所有资源并正确退出程序
    glfwTerminate();
}

int DrawTriangle_4()
{
    printf("\nDrawTriangle_3 : 绘制三角形。\n"
           "\t技术点:\n"
           "\t\t采用位置属性(vao + vbo + ebo):在顶点属性文件中设置;\n"
           "\t\t采用颜色属性:在顶点属性中为顶点指定颜色属性，在顶点着色器中读入颜色属性，传递给片段着色器;\n"
           "\t\t填充模式\n"
           "\t\t渲染: 采用vao中的ebo来渲染,glDrawElements\n");

    // 初始化工作
    GLFWwindow* window = Init();
    if (window == nullptr) return -1;

    // 注册按键回调
    // 回调注册时机：在窗口创建之后，渲染之前，都可以添加回调。
    glfwSetKeyCallback(window, key_callback);

    ShaderProgram shaderProgram("../res/shader/4_triangle/vertex.glsl",
                                "../res/shader/4_triangle/fragment.glsl");

    // 读取顶点属性数据
    std::vector<VertexData> vertexDatas;
    ReadVertexAttr_4(vertexDatas);
    const int vboNum = vertexDatas.size();

    // 顶点对象的封装。包括vao、vbo、ebo创建、vao、vbo、ebo绑定、vbo、ebo数据拷贝、设置顶点属性、解绑。
    std::vector<GLuint> vaos(vboNum), vbos(vboNum), ebos(vboNum);
    ConfigVertexAttr_4(vaos, vbos, ebos, vertexDatas);

    // 为了通用，为每一个顶点缓冲对象，配置一个着色器程序
    // 这里只用一个着色器程序，如果有多个着色器程序，可以配置多个
    std::vector<ShaderProgram> shaderPrograms(vboNum, shaderProgram);

    // 线框模式
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 渲染处理
    RenderTriangle_4(window, shaderPrograms, vaos, vertexDatas);

    // 删除VAO, VBO, EBO，释放资源
    glDeleteVertexArrays(vboNum, vaos.data());
    glDeleteBuffers(vboNum, vbos.data());
    glDeleteBuffers(vboNum, ebos.data());

    // 终止绘制，释放所有资源
    glfwTerminate();
    return 0;
}

// 顶点属性：vao、vbo、ebo
// 纹理属性:
int DrawRectangle_5()
{
    printf("\nDrawRectangle_5 : 绘制四边形。\n"
           "\t技术点:\n"
           "\t\t采用位置属性(vao + vbo + ebo):在顶点属性文件中设置;\n"
           "\t\t采用纹理属性: 纹理坐标在顶点属性文件中设置，纹理像素从图片中加载，并将纹理像素绑定到纹理对象中，实际输出像素颜色=纹理采样器（关联纹理对象） + 纹理坐标\n"
           "\t\t填充模式\n"
           "\t\t渲染: 采用vao中的ebo来渲染,glDrawElements\n");

    // 初始化工作
    GLFWwindow* window = Init();

    // 按键回调：在窗口创建之后，渲染之前，都可以添加回调。
    glfwSetKeyCallback(window, key_callback);

    // 着色器程序
    ShaderProgram shaderProgram("../res/shader/5_rectangle/vertex.glsl",
                                "../res/shader/5_rectangle/fragment.glsl");

    // 读取纹理像素并生成纹理
    GLuint texture;
    ReadAndConfigTexture_5(texture);

    // 从文件中读取顶点属性数据
    VertexData vertexData;
    ReadVertexAttr_5(vertexData);

    // 顶点对象的封装。包括vao、vbo、ebo创建、vao、vbo、ebo绑定、vbo、ebo数据拷贝、设置顶点属性、解绑。
    GLuint vao, vbo, ebo;
    ConfigVertexAttr_5(vao, vbo, ebo, vertexData);

    // 为了通用，为每一个顶点缓冲对象，配置一个着色器程序
    // 这里只用一个着色器程序，如果有多个着色器程序，可以配置多个

    // 线框模式
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 渲染处理
    RenderRectangle_5(window, shaderProgram, vao, texture, vertexData);

    // 删除VAO, VBO, EBO，释放资源
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glDeleteTextures(1, &texture);

    // 终止绘制，释放所有资源
    glfwTerminate();
    return 0;
}

int DrawRectangle_6()
{
    printf("\nDrawRectangle_6 : 绘制四边形。\n"
           "\t技术点:\n"
           "\t\t采用位置属性(vao + vbo + ebo):在顶点属性文件中设置;\n"
           "\t\t采用纹理属性: 使用2个图片来生成纹理，纹理坐标在顶点属性文件中设置，纹理像素从图片中加载，并将纹理像素绑定到纹理对象中，实际输出像素颜色=纹理采样器（关联纹理对象） + 纹理坐标\n"
           "\t\t填充模式\n"
           "\t\t渲染: 采用vao中的ebo来渲染,glDrawElements\n");

    // 初始化工作
    GLFWwindow* window = Init();

    // 按键回调：在窗口创建之后，渲染之前，都可以添加回调。
    glfwSetKeyCallback(window, key_callback_6);

    // 着色器程序
    ShaderProgram shaderProgram("../res/shader/6_rectangle/vertex.glsl",
                                "../res/shader/6_rectangle/fragment.glsl");

    // 读取纹理像素并生成纹理
    std::vector<GLuint> textures;
    ReadAndConfigTexture_6(textures);

    // 从文件中读取顶点属性数据
    VertexData vertexData;
    ReadVertexAttr_6(vertexData);

    // 顶点对象的封装。包括vao、vbo、ebo创建、vao、vbo、ebo绑定、vbo、ebo数据拷贝、设置顶点属性、解绑。
    GLuint vao, vbo, ebo;
    ConfigVertexAttr_6(vao, vbo, ebo, vertexData);

    // 为了通用，为每一个顶点缓冲对象，配置一个着色器程序
    // 这里只用一个着色器程序，如果有多个着色器程序，可以配置多个

    // 线框模式
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 渲染处理
    RenderRectangle_6(window, shaderProgram, vao, textures, vertexData);

    // 删除VAO, VBO, EBO，释放资源
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glDeleteTextures(textures.size(), textures.data());

    // 终止绘制，释放所有资源
    glfwTerminate();
    return 0;
}

int DrawRectangle_7()
{
    printf("\nDrawRectangle_7 : 绘制四边形。\n"
           "\t技术点:\n"
           "\t\t采用位置属性(vao + vbo + ebo):在顶点属性文件中设置;\n"
           "\t\t采用纹理属性: 使用2个图片来生成纹理，纹理坐标在顶点属性文件中设置，纹理像素从图片中加载，并将纹理像素绑定到纹理对象中，实际输出像素颜色=纹理采样器（关联纹理对象） + 纹理坐标\n"
           "\t\t\t纹理坐标超过范围[0,1]时，设置纹理的环绕方式，默认是重复\n"
           "\t\t填充模式\n"
           "\t\t渲染: 采用vao中的ebo来渲染,glDrawElements\n");

    // 初始化工作
    GLFWwindow* window = Init();

    // 按键回调：在窗口创建之后，渲染之前，都可以添加回调。
    glfwSetKeyCallback(window, key_callback);

    // 着色器程序
    ShaderProgram shaderProgram("../res/shader/7_rectangle/vertex.glsl",
                                "../res/shader/7_rectangle/fragment.glsl");

    // 读取纹理像素并生成纹理
    std::vector<GLuint> textures;
    ReadAndConfigTexture_7(textures);

    // 从文件中读取顶点属性数据
    VertexData vertexData;
    ReadVertexAttr_7(vertexData);

    // 顶点对象的封装。包括vao、vbo、ebo创建、vao、vbo、ebo绑定、vbo、ebo数据拷贝、设置顶点属性、解绑。
    GLuint vao, vbo, ebo;
    ConfigVertexAttr_7(vao, vbo, ebo, vertexData);

    // 为了通用，为每一个顶点缓冲对象，配置一个着色器程序
    // 这里只用一个着色器程序，如果有多个着色器程序，可以配置多个

    // 线框模式
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 渲染处理
    RenderRectangle_6(window, shaderProgram, vao, textures, vertexData);

    // 删除VAO, VBO, EBO，释放资源
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glDeleteTextures(textures.size(), textures.data());

    // 终止绘制，释放所有资源
    glfwTerminate();
    return 0;
}

int DrawRectangle_8()
{
    printf("\nDrawRectangle_7 : 绘制四边形。\n"
           "\t技术点:\n"
           "\t\t采用位置属性(vao + vbo + ebo):在顶点属性文件中设置;\n"
           "\t\t采用纹理属性: 使用2个图片来生成纹理，纹理坐标在顶点属性文件中设置，纹理像素从图片中加载，并将纹理像素绑定到纹理对象中，实际输出像素颜色=纹理采样器（关联纹理对象） + 纹理坐标\n"
           "\t\t\t通过按键响应回调，设置两个纹理像素混合的权重比值\n"
           "\t\t填充模式\n"
           "\t\t渲染: 采用vao中的ebo来渲染,glDrawElements\n");

    // 初始化工作
    GLFWwindow* window = Init();

    // 按键回调：在窗口创建之后，渲染之前，都可以添加回调。
    glfwSetKeyCallback(window, key_callback);

    // 着色器程序
    ShaderProgram shaderProgram("../res/shader/8_rectangle/vertex.glsl",
                                "../res/shader/8_rectangle/fragment.glsl");

    // 读取纹理像素并生成纹理
    std::vector<GLuint> textures;
    ReadAndConfigTexture_8(textures);

    // 从文件中读取顶点属性数据
    VertexData vertexData;
    ReadVertexAttr_8(vertexData);

    // 顶点对象的封装。包括vao、vbo、ebo创建、vao、vbo、ebo绑定、vbo、ebo数据拷贝、设置顶点属性、解绑。
    GLuint vao, vbo, ebo;
    ConfigVertexAttr_6(vao, vbo, ebo, vertexData);

    // 为了通用，为每一个顶点缓冲对象，配置一个着色器程序
    // 这里只用一个着色器程序，如果有多个着色器程序，可以配置多个

    // 线框模式
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 渲染处理
    RenderRectangle_6(window, shaderProgram, vao, textures, vertexData);

    // 删除VAO, VBO, EBO，释放资源
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glDeleteTextures(textures.size(), textures.data());

    // 终止绘制，释放所有资源
    glfwTerminate();
    return 0;
}

int main()
{
    //glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    //glm::mat4 trans = glm::mat4(1.0f);
    //trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    //vec = trans * vec;
    //std::cout << vec.x << vec.y << vec.z << std::endl;

    //return 0;

    DrawTriangle_1();

    DrawRectangle_2();

    DrawTriangle_3();

    DrawTriangle_4();

    DrawRectangle_5();

    DrawRectangle_6();

    DrawRectangle_7();
    DrawRectangle_8();

    return 0;
}