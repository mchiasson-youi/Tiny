#version 330 core

in vec4 color;
in vec2 texCoord;

uniform sampler2D texture0;

out vec4 FragColor;

void main()
{
    FragColor = color * texture(texture0, texCoord);
}