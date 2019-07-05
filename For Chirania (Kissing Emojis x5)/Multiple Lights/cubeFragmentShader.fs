#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct dirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct pointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct spotLight
{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float innerCutoff;
    float outerCutoff;
    float constant;
    float linear;
    float quadratic;
};

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

out vec4 final_color;

uniform Material mat;
uniform vec3 camPos;

#define NUM_DIRLIGHTS 20
#define NUM_POINTLIGHTS 20
#define NUM_SPOTLIGHTS 20

uniform dirLight dLights[NUM_DIRLIGHTS];
uniform int numDirLights;

uniform pointLight pLights[NUM_POINTLIGHTS];
uniform int numPointLights;

uniform spotLight sLights[NUM_SPOTLIGHTS];
uniform int numSpotLights;

vec3 calcDirLight(dirLight lMat, vec3 norm, vec3 viewDir)
{
    vec3 ambient = lMat.ambient * texture(mat.diffuse, texCoords).rgb;

    vec3 lightDir = normalize(-lMat.direction);
    vec3 diffuse = lMat.diffuse *  max(dot(norm, lightDir), 0.0f) * texture(mat.diffuse, texCoords).rgb;

    vec3 reflectedLightDir = reflect(-lightDir, norm);
    vec3 specular = lMat.specular * pow(max(dot(reflectedLightDir, viewDir), 0.0f), mat.shininess) * texture(mat.specular, texCoords).rgb;

    return (ambient + diffuse + specular);
}

vec3 calcPointLight(pointLight lMat, vec3 fragPos, vec3 norm, vec3 viewDir)
{
    vec3 lightDir = lMat.position - fragPos;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);
    float attenuation = lMat.constant + lMat.linear * distance + lMat.quadratic * distance * distance;
    attenuation = 1.0 / attenuation;

    vec3 ambient = lMat.ambient * texture(mat.diffuse, texCoords).rgb;

    vec3 diffuse = lMat.diffuse *  max(dot(norm, lightDir), 0.0f) * texture(mat.diffuse, texCoords).rgb;

    vec3 reflectedLightDir = reflect(-lightDir, norm);
    vec3 specular = lMat.specular * pow(max(dot(reflectedLightDir, viewDir), 0.0f), mat.shininess) * texture(mat.specular, texCoords).rgb;

    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation; 

    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(spotLight lMat, vec3 fragPos, vec3 norm, vec3 viewDir)
{
    vec3 lightDir = lMat.position - fragPos;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);
    float attenuation = lMat.constant + lMat.linear * distance + lMat.quadratic * distance * distance;
    attenuation = 1.0 / attenuation;
    float intensity = clamp(((dot(lightDir, normalize(-lMat.direction))) - lMat.outerCutoff) / (lMat.innerCutoff - lMat.outerCutoff), 0.0f, 1.0f);

    vec3 ambient = lMat.ambient * texture(mat.diffuse, texCoords).rgb;

    vec3 diffuse = lMat.diffuse *  max(dot(norm, lightDir), 0.0f) * texture(mat.diffuse, texCoords).rgb;

    vec3 reflectedLightDir = reflect(-lightDir, norm);
    vec3 specular = lMat.specular * pow(max(dot(reflectedLightDir, viewDir), 0.0f), mat.shininess) * texture(mat.specular, texCoords).rgb;

    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation; 

    diffuse *= intensity;
    specular *= intensity;

    return (ambient + diffuse + specular);
}

void main()
{
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(camPos - fragPos);

    vec3 result = vec3(0.0f, 0.0f, 0.0f);

    int num = min(numDirLights, NUM_DIRLIGHTS);
    for(int i = 0; i < num; i++)
        result += calcDirLight(dLights[i], norm, viewDir);

    num = min(numPointLights, NUM_POINTLIGHTS);
    for(int i = 0; i < num; i++)
        result += calcPointLight(pLights[i], fragPos, norm, viewDir);
    
    num = min(numSpotLights, NUM_SPOTLIGHTS);
    for(int i = 0; i < num; i++)
        result += calcSpotLight(sLights[i], fragPos, norm, viewDir);

    final_color = vec4(result, 1.0f);
}