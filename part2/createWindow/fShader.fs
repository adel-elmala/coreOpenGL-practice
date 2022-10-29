#version 330 core

uniform vec4 vColor;
in vec4 ourColor;
out vec4 color;

void main()
{
    color = vec4(  ourColor.r + vColor.g,
                ourColor.g,
                ourColor.b,
                ourColor.a);
};
