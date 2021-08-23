#version 330 core

uniform vec4 vertexColor;
out vec4 FragColor;

void main()
{
    // FragColor = vec4(0.1f, 0.8f, 0.1f, 1.0f);
    FragColor = vertexColor;
}