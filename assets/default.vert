#version 330 core
layout (location = 0) in vec3 aPos;

uniform vec2 screen_size;

void main()
{
    gl_Position = vec4((aPos.x/screen_size.x * 2.0 - 1.0), (aPos.y/screen_size.y * 2.0 - 1.0), aPos.z, 1.0);
}