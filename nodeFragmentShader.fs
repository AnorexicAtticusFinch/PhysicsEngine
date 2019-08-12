#version 330 core

in vec2 texCoord;

out vec4 final_color;

uniform vec3 color;

void main()
{
    final_color = vec4(color, 1.0f);
}