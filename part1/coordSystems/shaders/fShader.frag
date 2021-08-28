#version 330 core

in vec2 texCoord;
uniform sampler2D texture;
out vec4 fragColor;

void main()
{
    // fragColor = vec4(1.0f,0.0f,0.0f,1.0f);
    fragColor = texture2D(texture,texCoord);
}