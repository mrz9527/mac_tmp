#version 330 core

in vec3 ourColor;
in vec2 ourTexCoord;

out vec4 color;
// uniform关键字声明的变量，其值由应用程序来传入，而不是由前一个着色器来传入，所以uniform前面不需要加in，
// 也不需要加out，因为应用程序能直接获取uniform声明的变量，另外，uniform声明的变量往往是来构造其他out类型变量的，所以不需要加out
uniform sampler2D ourTexture1;   // sampler2D，采样器，用来采样纹理像素。
                                     // textureSamper2D变量在程序中没有直接赋值，是因为默认使用纹理单元GL_TEXTURE0
                                    // opengl保证至少有16个纹理单元供你使用。GL_TEXTURE0 ~ GL_TEXTURE15
uniform sampler2D ourTexture2;

void main()
{
    color = mix(texture(ourTexture1, ourTexCoord), texture(ourTexture2, ourTexCoord), 0.2); // 0.8 * 第一个纹理像素 + 0.2 * 第二个纹理像素
}