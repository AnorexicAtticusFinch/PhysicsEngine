#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct lightMaterial
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

out vec4 final_color;

uniform Material mat;
uniform lightMaterial lMat;

uniform vec3 camPos;

void main()
{
    //AMBIENT
    vec3 ambient = lMat.ambient * texture(mat.diffuse, texCoords).rgb;

    //DIFFUSE
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-lMat.direction);
    float diff = dot(norm, lightDir);
    diff = max(diff, 0.0f);
    vec3 diffuse = lMat.diffuse * diff * texture(mat.diffuse, texCoords).rgb;

    //SPECULAR
    vec3 viewDir = normalize(camPos - fragPos);
    vec3 reflectedLightDir = reflect(-lightDir, norm);
    float spec = dot(reflectedLightDir, viewDir);
    spec = max(spec, 0.0f);
    spec = pow(spec, mat.shininess);
    vec3 specular = lMat.specular * spec * (texture(mat.specular, texCoords).rgb + vec3(0.05f, 0.05f, 0.05f));

    final_color = vec4(ambient + diffuse + specular, 1.0f);
}