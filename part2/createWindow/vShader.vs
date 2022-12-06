#version 330 core

layout (location = 0) in vec3 vpos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// out vec2 TexCoord; 
out vec3 normal;
out vec3 fragPos;
out vec2 vTexture;
void main()
{
    vec4 vw = model * vec4(vpos,1.0f) ; 
    gl_Position = projection * view * vw;
    fragPos = vec3(vw.xyz);
    // TexCoord = vTex;
    normal = mat3(transpose(inverse(model))) * vNormal;
    vTexture = vTex;
}