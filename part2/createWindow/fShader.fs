#version 330 core

uniform vec4 vColor;
uniform sampler2D tex;

in vec4 ourColor;
in vec2 TexCoord;

out vec4 color;


void main()
{
    color = texture(tex,TexCoord);
    // color = vec4(  ourColor.r + vColor.g,
    //             ourColor.g,
    //             ourColor.b,
    //             ourColor.a);
};
