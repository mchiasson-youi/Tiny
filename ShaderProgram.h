#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glad/glad.h>
#include "Shader.h"


class ShaderProgram
{
    GLuint handle = 0;
    GLint screen_size_loc = -1;

public:
    ShaderProgram()
    {
        handle = glCreateProgram();
    }

    ~ShaderProgram()
    {
        if (handle)
        {
            glDeleteProgram(handle);
            handle = 0;
        }
    }

    void attach(Shader *shader)
    {
        glAttachShader(handle, shader->getHandle());
    }

    int link()
    {
        glLinkProgram(handle);

        GLint linkStatus;
        glGetProgramiv(handle, GL_LINK_STATUS, &linkStatus);
        if (!linkStatus)
        {
            GLchar infoLog[1024];
            glGetProgramInfoLog(handle, sizeof(infoLog), NULL, infoLog);
            SDL_LogCritical(0, "Could not link shader program:\n%s", infoLog);
            return -1;
        }

        screen_size_loc = glGetUniformLocation(handle, "screen_size");

        return 0;
    }

    void activate()
    {
        glUseProgram(handle);
    }

    void deactivate()
    {
        glUseProgram(0);
    }

    void set_screen_size(float width, float height)
    {
        glUniform2f(screen_size_loc, width, height);
    }

};



#endif