#version 330 core

out vec4 final_color;

uniform vec3 lightDiffuseColor;

void main()
{
    final_color = vec4(lightDiffuseColor, 1.0f);
}