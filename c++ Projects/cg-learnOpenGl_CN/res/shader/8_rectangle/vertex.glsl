#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec2 ourTexCoord;

void main()
{
    gl_Position = vec4(position, 1.0f);
    ourColor = color;
//    ourTexCoord = texCoord;
    ourTexCoord = vec2(texCoord.x, 1.0f - texCoord.y); // 默认图片的原点在图片的左上角，图片的y轴向下，而opengl中，要求y轴向上，所以对y轴做一个反转
}