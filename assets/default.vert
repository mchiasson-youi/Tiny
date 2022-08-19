#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 projection;
uniform mat4 model;

out vec4 color;
out vec2 texCoord;

void main()
{
    color = aColor;
    texCoord = aTexCoord;
    gl_Position = projection * model * vec4(aPos.xyz, 1.0);
}