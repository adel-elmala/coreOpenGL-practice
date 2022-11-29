#version 330 core

layout (location = 0) in vec3 vpos;
// layout (location = 1) in vec3 vcol;
layout (location = 1) in vec2 vTex;
uniform mat4 trans;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// out vec4 ourColor;
out vec2 TexCoord; 

void main()
{
    // gl_Position = trans * vec4(vpos,1.0f);
    gl_Position = projection * view * model * vec4(vpos,1.0f);
    // ourColor = vec4(vcol,1.0f);
    TexCoord = vTex;
};