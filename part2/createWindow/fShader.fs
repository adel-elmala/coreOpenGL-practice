#version 330 core


in vec3 normal;
in vec3 fragPos;
in vec2 vTexture;

out vec4 color;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shineness;
};

struct Light 
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};

uniform vec3 cameraPos;
uniform Material mat;
uniform Light light;

void main()
{
    // vec3 objectColor = vec3(0.5f,0.3f,0.4f);
    
    // ambinet comp.    
    vec3 ambientColor = light.ambient * vec3(texture(mat.diffuse,vTexture)) ;

    // diffuse comp. 
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 n = normalize(normal);
    vec3 diffuseColor = max(dot(n,lightDir),0.0) * vec3(texture(mat.diffuse,vTexture)) * light.diffuse;
    
    // specular comp. 
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectionV = normalize(reflect(-lightDir,normal));
    float spec = pow(max(dot(reflectionV,viewDir),0.0f),mat.shineness);
    vec3 specularColor =  spec * light.specular * texture(mat.specular,vTexture).rgb;
    
    color =  vec4((diffuseColor + ambientColor + specularColor)  ,1.0) ;

}
