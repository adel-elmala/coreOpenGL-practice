#version 330 core

layout (location = 0) in vec3 vpos;
layout (location = 1) in vec3 vcol;
layout (location = 1) in vec2 vTex;

out vec4 ourColor;
out vec2 TexCoord; 
void main()
{
    gl_Position = vec4(vpos,1.0f);
    ourColor = vec4(vcol,1.0f);
    TexCoord = vTex;
};