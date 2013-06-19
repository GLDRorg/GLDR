#pragma once
#include <GL/glew.h>
#include <utility>
#include "GLId.h"

namespace gldr {

class VertexBuffer
{
public:
    enum class Type
    {
        DATA_BUFFER = GL_ARRAY_BUFFER,
        INDEX_BUFFER = GL_ELEMENT_ARRAY_BUFFER
    };
    enum class Usage
    {
        DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
        DYNAMIC_READ = GL_DYNAMIC_READ,
        STATIC_DRAW = GL_STATIC_DRAW,
        STATIC_READ = GL_STATIC_READ,
        STREAM_DRAW = GL_STREAM_DRAW,
        STREAM_READ = GL_STREAM_READ,
    };
    
private:
    GLId m_VBOId;
    GLId m_savedVBOId;

    Usage m_Usage;
    Type m_Type;

protected:
    void _GenerateVBOId();
    void _GuardedBind();
    void _GuardedUnbind();
    void _UpdateCachedValue();
public:
    void Bind();
    void LoadData (const void* data, int size);

    VertexBuffer(Type type = Type::DATA_BUFFER, Usage usage = Usage::STATIC_DRAW);
    VertexBuffer(VertexBuffer&& other) :
        m_VBOId(std::move(other.m_VBOId)),
        m_savedVBOId(std::move(other.m_savedVBOId)),
        m_Usage(other.m_Usage),
        m_Type(other.m_Type)
    { }
    ~VertexBuffer();
};

} // namespace gldr
