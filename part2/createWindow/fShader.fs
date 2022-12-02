#version 330 core

uniform vec4 vColor;
uniform sampler2D tex;
uniform sampler2D tex2;


in vec2 TexCoord;
out vec4 color;


void main()
{
    color = mix(texture(tex,TexCoord),
                texture(tex2,TexCoord),
                0.5f);
};
