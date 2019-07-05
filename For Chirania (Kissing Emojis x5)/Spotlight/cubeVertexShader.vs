#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal_in;
layout (location = 2) in vec2 texCoords_in;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoords;

uniform mat4 transform;
uniform mat4 model;
uniform mat3 normalMatrix;

void main()
{
    gl_Position = transform * vec4(position, 1.0f);
    fragPos = position; //Multiply by model
    normal = normal_in; //Multiply by normalMatrix ( normalMatrix = mat3( transpose( inverse( model ) ) ) )
    texCoords = texCoords_in;
}