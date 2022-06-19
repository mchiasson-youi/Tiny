#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

struct Vertex
{
    glm::vec3 aPos;
};

class VertexBuffer
{
    GLuint vao = 0;
    GLuint vbo = 0;

public:

    enum Hint
    {
        Stream  = GL_STREAM_DRAW,
        Static  = GL_STATIC_DRAW,
        Dynamic = GL_DYNAMIC_DRAW
    };

    VertexBuffer()
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        // record
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, aPos));
        glBindVertexArray(0);
    }

    ~VertexBuffer()
    {
        glDeleteVertexArrays(1, &vao);
        vao = 0;

        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }

    void activate()
    {
        glBindVertexArray(vao);
    }

    void deactivate()
    {
        glBindVertexArray(0);
    }

    void upload(const std::vector<Vertex> &vertices, Hint hint = Static)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), hint);
    }

};


#endif
