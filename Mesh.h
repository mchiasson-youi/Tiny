#ifndef MESH_H
#define MESH_H

#include <memory>
#include "VertexBuffer.h"

namespace Primitive
{
    enum PrimitiveTypes
    {
        Point = GL_POINTS,
        LineList = GL_LINES,
        LineStrip = GL_LINE_STRIP,
        TriangleList = GL_TRIANGLES,
        TriangleStrip = GL_TRIANGLE_STRIP,
    };
}


class Mesh
{
protected:

    std::shared_ptr<VertexBuffer> vertexBuffer;

    Primitive::PrimitiveTypes primitive = Primitive::TriangleList;
    int32_t vertexStartOffset = 0;
    int32_t vertexCount = 0;

public:

    const std::shared_ptr<VertexBuffer> &getVertexBuffer() const { return vertexBuffer; }
    void setVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) { this->vertexBuffer = vertexBuffer; }

    Primitive::PrimitiveTypes getPrimitive() const
    {
        return primitive;
    }

    void setPrimitive(Primitive::PrimitiveTypes primitive)
    {
        this->primitive = primitive;
    }

    int32_t getVertexStartOffset() const
    {
        return vertexStartOffset;
    }

    void setVertexStartOffset(int32_t vertexStartOffset)
    {
        this->vertexStartOffset = vertexStartOffset;
    }

    int32_t getVertexCount() const
    {
        return vertexCount;
    }

    void setVertexCount(int32_t vertexCount)
    {
        this->vertexCount = vertexCount;
    }

};

#endif /* MESH_H */