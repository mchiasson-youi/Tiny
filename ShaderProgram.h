#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glad/glad.h>
#include "Shader.h"


class ShaderProgram
{
    GLuint handle = 0;

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

};



#endif