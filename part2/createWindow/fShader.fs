#version 330 core

uniform vec4 vColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

// uniform sampler2D tex;
// uniform sampler2D tex2;

in vec3 normal;
in vec3 fragPos;
// in vec2 TexCoord;
out vec4 color;


void main()
{
    vec3 objectColor = vec3(0.5f,0.3f,0.4f);
    
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 n = normalize(normal);
    vec3 diff = max(dot(n,lightDir),0.0) * lightColor;

    float ambientIntensity = 0.2f;

    vec3 ambientColor = ambientIntensity * lightColor ;
    
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectionV = normalize(reflect(-lightDir,normal));
    float spec = pow(max(dot(reflectionV,viewDir),0.0f),32);
    float specularIntensity = 0.5;
    vec3 specular = lightColor * spec * specularIntensity;
    
    color =  vec4((diff + ambientColor + specular) * objectColor,1.0) ;

    // color =  vec4(ambientColor,1.0f) * mix(texture(tex,TexCoord),
    //             texture(tex2,TexCoord),
    //             0.5f);

};
