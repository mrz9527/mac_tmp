#version 330 core

in vec3 ourColor;
in vec2 ourTexCoord;

out vec4 color;
// uniform关键字声明的变量，其值由应用程序来传入，而不是由前一个着色器来传入，所以uniform前面不需要加in，
// 也不需要加out，因为应用程序能直接获取uniform声明的变量，另外，uniform声明的变量往往是来构造其他out类型变量的，所以不需要加out
uniform sampler2D textureSamper2D;   // sampler2D，采样器，用来采样纹理像素。
                                     // textureSamper2D变量在程序中没有直接赋值，是因为默认使用纹理单元GL_TEXTURE0
                                    // opengl保证至少有16个纹理单元供你使用。GL_TEXTURE0 ~ GL_TEXTURE15

void main()
{
//    color = texture(textureSamper2D, ourTexCoord);  // 基于采样器，使用纹理坐标获取纹理颜色叫做采样，来确定输出的像素颜色
    color = texture(textureSamper2D, ourTexCoord) * vec4(ourColor, 1.0); // 纹理颜色和自定义颜色混合
}