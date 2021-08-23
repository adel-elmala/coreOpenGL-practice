#version 330 core

//in vec3 pixelColor;
in vec4 vPos;
out vec4 FragColor;

void main()
{
    // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    // FragColor = vec4(pixelColor, 1.0f);
    FragColor = vPos;
}