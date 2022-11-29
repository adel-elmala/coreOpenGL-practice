#version 330 core

uniform vec4 vColor;
uniform sampler2D tex;
uniform sampler2D tex2;

// in vec4 ourColor;
in vec2 TexCoord;

out vec4 color;


void main()
{
    color = mix(texture(tex,TexCoord),
                texture(tex2,TexCoord),
                vColor.g);
    // color = vec4(  ourColor.r + vColor.g,
    //             ourColor.g,
    //             ourColor.b,
    //             ourColor.a);
};
