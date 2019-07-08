#ifndef SHADER_H
#define SHADER_H

#include "./glad.c"
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct simpleMaterial
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct Material
{
    int diffuse; //Texture Unit
    int specular; //Texture Unit
    float shininess;
};

class Light
{
    public:

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        Light();
        Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
};

Light::Light()
{   
}

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
    this -> ambient = ambient;
    this -> diffuse = diffuse;
    this -> specular = specular;
}

class pointLight : public Light
{
    public:

        glm::vec3 position;
        float constant;
        float linear;
        float quadratic;

        pointLight();
        pointLight(glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
};

pointLight::pointLight()
{   
}

pointLight::pointLight(glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : 
    Light(ambient, diffuse, specular)
{
    this -> position = position;
    this -> constant = constant;
    this -> linear = linear;
    this -> quadratic = quadratic;
}

class dirLight : public Light
{
    public:

        glm::vec3 direction;

        dirLight();
        dirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
};

dirLight::dirLight()
{   
}

dirLight::dirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : 
    Light(ambient, diffuse, specular)
{
    this -> direction = direction;
}

class spotLight : public pointLight
{
    public:

        glm::vec3 direction;
        float innerCutoff;
        float outerCutoff;

        spotLight();
        spotLight(glm::vec3 position, glm::vec3 direction, float innerCutoff, float outerCutoff, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
};

spotLight::spotLight()
{   
}

spotLight::spotLight(glm::vec3 position, glm::vec3 direction, float innerCutoff, float outerCutoff, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : 
    pointLight(position, constant, linear, quadratic, ambient, diffuse, specular)
{
    this -> direction = direction;
    this -> innerCutoff = innerCutoff;
    this -> outerCutoff = outerCutoff;
}

unsigned int get2DTexID(const char* imagePath, bool flipVertically = false)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(flipVertically);
    unsigned char* data = stbi_load(imagePath, &width, &height, &nrComponents, 0);

    GLenum format;
    if (nrComponents == 1)
        format = GL_RED;
    else if (nrComponents == 3)
        format = GL_RGB;
    else if (nrComponents == 4)
        format = GL_RGBA;

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        cout << "Error while loading Texture" << endl;
    
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

class Shader
{
        unsigned int programID;

    public:

        Shader(const char* vertexFilePath, const char* fragmentFilePath);
        Shader(const char* vertexFilePath, const char* geometryFilePath, const char* fragmentFilePath);

        unsigned int getProgramID() const;
        int getUniformLocation(const char* uniformName) const;
        void use() const;

        int setIntUniform(const char* uniformName, int val) const;
        int setFloatUniform(const char* uniformName, float val) const;
        int setFVec3Uniform(const char* uniformName, float val1, float val2, float val3) const;
        int setFVec3Uniform(const char* uniformName, glm::vec3 vector) const;
        int setFVec4Uniform(const char* uniformName, float val1, float val2, float val3, float val4) const;
        int setFVec4Uniform(const char* uniformName, glm::vec4 vector) const;
        int setFMat3Uniform(const char* uniformName, glm::mat3 matrix) const;
        int setFMat4Uniform(const char* uniformName, glm::mat4 matrix) const;
        int setSimpleMaterialUniform(const char* uniformName, simpleMaterial mat) const;
        int setMaterialUniform(const char* uniformName, Material mat) const;
        int setPointLightMaterialUniform(const char* uniformName, pointLight mat) const;
        int setDirLightMaterialUniform(const char* uniformName, dirLight mat) const;
        int setSpotLightMaterialUniform(const char* uniformName, spotLight mat) const;
};

Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath)
{
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    ifstream vertexFile, fragmentFile;
    vertexFile.open(vertexFilePath);
    fragmentFile.open(fragmentFilePath);

    string vertexSource, fragmentSource, code;

    while (! vertexFile.eof())
    {
        getline(vertexFile, code);
        vertexSource += code + "\n";
    }
    vertexSource += "\0";

    while (! fragmentFile.eof())
    {
        getline(fragmentFile, code);
        fragmentSource += code + "\n";
    }
    fragmentSource += "\0";

    vertexFile.close();
    fragmentFile.close();
    
    const char* vertexCode = vertexSource.c_str();
    const char* fragmentCode = fragmentSource.c_str();

    int success;
    char infoLog[512];

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (! success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        cout << "Error while compiling Vertex Shader: " << infoLog << endl;
    }

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (! success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        cout << "Error while compiling Fragment Shader: " << infoLog << endl;
    }

    programID = glCreateProgram();
    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (! success)
    {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        cout << "Error while linking Shaders: " << infoLog << endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);   
}

Shader::Shader(const char* vertexFilePath, const char* geometryFilePath, const char* fragmentFilePath)
{
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    ifstream vertexFile, fragmentFile, geometryFile;
    vertexFile.open(vertexFilePath);
    fragmentFile.open(fragmentFilePath);
    geometryFile.open(geometryFilePath);

    string vertexSource, fragmentSource, code, geometrySource;

    while (! vertexFile.eof())
    {
        getline(vertexFile, code);
        vertexSource += code + "\n";
    }
    vertexSource += "\0";

    while (! fragmentFile.eof())
    {
        getline(fragmentFile, code);
        fragmentSource += code + "\n";
    }
    fragmentSource += "\0";

    while (! geometryFile.eof())
    {
        getline(geometryFile, code);
        geometrySource += code + "\n";
    }
    geometrySource += "\0";

    vertexFile.close();
    fragmentFile.close();
    geometryFile.close();
    
    const char* vertexCode = vertexSource.c_str();
    const char* fragmentCode = fragmentSource.c_str();
    const char* geometryCode = geometrySource.c_str();

    int success;
    char infoLog[512];

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (! success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        cout << "Error while compiling Vertex Shader: " << infoLog << endl;
    }

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (! success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        cout << "Error while compiling Fragment Shader: " << infoLog << endl;
    }

    unsigned int geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &geometryCode, NULL);
    glCompileShader(geometry);

    glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
    if (! success)
    {
        glGetShaderInfoLog(geometry, 512, NULL, infoLog);
        cout << "Error while compiling Geometry Shader: " << infoLog << endl;
    }

    programID = glCreateProgram();
    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    glAttachShader(programID, geometry);
    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (! success)
    {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        cout << "Error while linking Shaders: " << infoLog << endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment); 
    glDeleteShader(geometry);  
}

unsigned int Shader::getProgramID() const
{
    return programID;
}

int Shader::getUniformLocation(const char* uniformName) const
{
    return glGetUniformLocation(programID, uniformName);
}

void Shader::use() const
{
    glUseProgram(programID);
}

int Shader::setIntUniform(const char* uniformName, int val) const
{
    int location = glGetUniformLocation(programID, uniformName);
    
    if (location == -1)
        return 0;

    glUniform1i(location, val);
    
    return 1;
}

int Shader::setFloatUniform(const char* uniformName, float val) const
{
    int location = glGetUniformLocation(programID, uniformName);
    
    if (location == -1)
        return 0;

    glUniform1f(location, val);
    
    return 1;
}

int Shader::setFVec3Uniform(const char* uniformName, float val1, float val2, float val3) const
{
    int location = glGetUniformLocation(programID, uniformName);
    
    if (location == -1)
        return 0;

    glUniform3f(location, val1, val2, val3);
    
    return 1;
}

int Shader::setFVec3Uniform(const char* uniformName, glm::vec3 vector) const
{
    int location = glGetUniformLocation(programID, uniformName);
    
    if (location == -1)
        return 0;

    glUniform3fv(location, 1, &vector[0]);
    
    return 1;
}

int Shader::setFVec4Uniform(const char* uniformName, float val1, float val2, float val3, float val4) const
{
    int location = glGetUniformLocation(programID, uniformName);
    
    if (location == -1)
        return 0;

    glUniform4f(location, val1, val2, val3, val4);
    
    return 1;
}

int Shader::setFVec4Uniform(const char* uniformName, glm::vec4 vector) const
{
    int location = glGetUniformLocation(programID, uniformName);
    
    if (location == -1)
        return 0;

    glUniform4fv(location, 1, &vector[0]);
    
    return 1;
}

int Shader::setFMat3Uniform(const char* uniformName, glm::mat3 matrix) const
{
    int location = glGetUniformLocation(programID, uniformName);
    
    if (location == -1)
        return 0;

    glUniformMatrix3fv(location, 1, GL_FALSE, &matrix[0][0]);
    
    return 1;
}

int Shader::setFMat4Uniform(const char* uniformName, glm::mat4 matrix) const
{
    int location = glGetUniformLocation(programID, uniformName);
    
    if (location == -1)
        return 0;

    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
    
    return 1;
}

int Shader::setSimpleMaterialUniform(const char* uniformName, simpleMaterial mat) const
{
    string name = uniformName;

    int success = setFVec3Uniform((name + ".ambient").c_str(), mat.ambient);
    success *= setFVec3Uniform((name + ".diffuse").c_str(), mat.diffuse);
    success *= setFVec3Uniform((name + ".specular").c_str(), mat.specular);
    success *= setFloatUniform((name + ".shininess").c_str(), mat.shininess);

    return success;
}

int Shader::setMaterialUniform(const char* uniformName, Material mat) const
{
    string name = uniformName;

    int success = setIntUniform((name + ".diffuse").c_str(), mat.diffuse);
    success *= setIntUniform((name + ".specular").c_str(), mat.specular);
    success *= setFloatUniform((name + ".shininess").c_str(), mat.shininess);

    return success;
}

int Shader::setPointLightMaterialUniform(const char* uniformName, pointLight mat) const
{
    string name = uniformName;

    int success = setFVec3Uniform((name + ".ambient").c_str(), mat.ambient);
    success *= setFVec3Uniform((name + ".diffuse").c_str(), mat.diffuse);
    success *= setFVec3Uniform((name + ".specular").c_str(), mat.specular);
    success *= setFVec3Uniform((name + ".position").c_str(), mat.position);
    success *= setFloatUniform((name + ".constant").c_str(), mat.constant);
    success *= setFloatUniform((name + ".linear").c_str(), mat.linear);
    success *= setFloatUniform((name + ".quadratic").c_str(), mat.quadratic);

    return success;
}

int Shader::setDirLightMaterialUniform(const char* uniformName, dirLight mat) const
{
    string name = uniformName;

    int success = setFVec3Uniform((name + ".ambient").c_str(), mat.ambient);
    success *= setFVec3Uniform((name + ".diffuse").c_str(), mat.diffuse);
    success *= setFVec3Uniform((name + ".specular").c_str(), mat.specular);
    success *= setFVec3Uniform((name + ".direction").c_str(), mat.direction);

    return success;
}

int Shader::setSpotLightMaterialUniform(const char* uniformName, spotLight mat) const
{
    string name = uniformName;

    int success = setFVec3Uniform((name + ".ambient").c_str(), mat.ambient);
    success *= setFVec3Uniform((name + ".diffuse").c_str(), mat.diffuse);
    success *= setFVec3Uniform((name + ".specular").c_str(), mat.specular);
    success *= setFVec3Uniform((name + ".position").c_str(), mat.position);
    success *= setFVec3Uniform((name + ".direction").c_str(), mat.direction);
    success *= setFloatUniform((name + ".innerCutoff").c_str(), mat.innerCutoff);
    success *= setFloatUniform((name + ".outerCutoff").c_str(), mat.outerCutoff);
    success *= setFloatUniform((name + ".constant").c_str(), mat.constant);
    success *= setFloatUniform((name + ".linear").c_str(), mat.linear);
    success *= setFloatUniform((name + ".quadratic").c_str(), mat.quadratic);

    return success;
}

#endif
