#version 330 core

in vec3 pixelColor;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D Tex1;
uniform sampler2D Tex2;

void main()
{
    // FragColor = vec4(pixelColor, 1.0f);
    FragColor = mix(texture(Tex1,texCoord),
                    texture(Tex2,texCoord),
                    0.3);
}