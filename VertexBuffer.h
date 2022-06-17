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
    GLuint handle;

public:

    enum Hint
    {
        Stream  = GL_STREAM_DRAW,
        Static  = GL_STATIC_DRAW,
        Dynamic = GL_DYNAMIC_DRAW
    };

    VertexBuffer()
    {
        glGenBuffers(1, &handle);
    }

    ~VertexBuffer()
    {
        glDeleteBuffers(1, &handle);
        handle = 0;
    }

    void activate()
    {
        glBindBuffer(GL_ARRAY_BUFFER, handle);
    }

    void deactivate()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void upload(const std::vector<Vertex> &vertices, Hint hint = Static)
    {
        glBindBuffer(GL_ARRAY_BUFFER, handle);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), hint);
    }

};


#endif
