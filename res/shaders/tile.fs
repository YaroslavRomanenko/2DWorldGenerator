#version 420 core
out vec4 FragColor;

in vec2 vTexCoords;
in float vTexIndex;

uniform sampler2D uTextures[6];

void main() 
{
    int index = int(vTexIndex);

    FragColor = texture(uTextures[index], vTexCoords);
}