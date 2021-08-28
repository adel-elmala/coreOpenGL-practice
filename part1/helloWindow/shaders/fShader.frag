#version 410 core

in vec3 pixelColor;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D Tex1;
uniform sampler2D Tex2;
uniform float alpha;

void main()
{
    
    // FragColor = vec4(pixelColor, 1.0f);
    FragColor = mix(texture2D(Tex1,texCoord),
                    texture2D(Tex2,texCoord),
                    alpha);
}