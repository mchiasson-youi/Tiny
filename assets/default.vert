#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 projection;
uniform mat4 model;

out vec4 color;

void main()
{
    color = aColor;
    gl_Position = projection * model * vec4(aPos.xyz, 1.0);
}