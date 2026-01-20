#version 420 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in float aTexIndex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 vTexCoords;
out float vTexIndex;

void main() 
{
    gl_Position = projection * view * model * vec4(aPos, 0.0f, 1.0f);
    vTexCoords = aTexCoords;
    vTexIndex = aTexIndex;
}