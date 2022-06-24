#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glad/glad.h>
#include "Shader.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>


class ShaderProgram
{
    GLuint handle = 0;
    GLint projection_loc = -1;
    GLint model_loc = -1;

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

        projection_loc = glGetUniformLocation(handle, "projection");
        model_loc = glGetUniformLocation(handle, "model");

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

    void setProjection(const glm::mat4 &model)
    {
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(model));
    }

    void setModel(const glm::mat4 &model)
    {
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    }

};



#endif