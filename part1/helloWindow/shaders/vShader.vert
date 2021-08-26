#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 atexCoord;

out vec2 texCoord;
out vec3 pixelColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    pixelColor = aColor;
    texCoord = atexCoord;
}