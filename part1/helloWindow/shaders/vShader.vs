#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec4 vPos;
//out vec3 pixelColor;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    //pixelColor = aColor;
    vPos = gl_Position;
}