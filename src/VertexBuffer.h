#pragma once
#include <GL/glew.h>
#include <utility>
#include <stdexcept>
#include "GLId.h"

namespace gldr {

namespace {
    GLuint vboCreate () { 
        GLuint id = 0;
        glGenFramebuffers(1, &id);
        if (!id)
            throw std::runtime_error("Problem creating a texture");
        return id;
    }

    void vboDelete(GLuint id) {
        glDeleteFramebuffers(1, &id);
    }

    class VboGLId : public GLId<decltype(&vboDelete)> {
    public:
        VboGLId()
            : GLId(vboCreate(), vboDelete)
        { }
        VboGLId(GLuint id)
            : GLId(id, vboDelete)
        { }
    };
}

class VertexBuffer
{
public:
    enum class Type : GLenum
    {
        DATA_BUFFER = GL_ARRAY_BUFFER,
        INDEX_BUFFER = GL_ELEMENT_ARRAY_BUFFER
    };
    enum class Usage : GLenum
    {
        DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
        DYNAMIC_READ = GL_DYNAMIC_READ,
        STATIC_DRAW = GL_STATIC_DRAW,
        STATIC_READ = GL_STATIC_READ,
        STREAM_DRAW = GL_STREAM_DRAW,
        STREAM_READ = GL_STREAM_READ,
    };
    
private:
    VboGLId id;
    VboGLId savedId;

    Usage usage;
    Type type;

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
        id(std::move(other.id)),
        savedId(std::move(other.savedId)),
        usage(other.usage),
        type(other.type)
    { }
    ~VertexBuffer();
};


#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

void VertexBuffer::_UpdateCachedValue()
{
    int temp;
    switch (type)
    {
    case Type::DATA_BUFFER:
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &temp);
        break;
    case Type::INDEX_BUFFER:
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &temp);
        break;
    }

    savedId = temp;
}

void VertexBuffer::_GuardedBind()
{
    _UpdateCachedValue();

    if(id != savedId)
        glBindBuffer(static_cast<GLenum>(type), id);
}

void VertexBuffer::_GuardedUnbind()
{
    if(id != savedId)
        glBindBuffer(static_cast<GLenum>(type), savedId);
}

void VertexBuffer::Bind()
{
    glBindBuffer(static_cast<GLenum>(type), id);
}

void VertexBuffer::LoadData(const void* data, int size)
{
    _GuardedBind();
    glBufferData(static_cast<GLenum>(type), size, data, static_cast<GLenum>(usage));
    _GuardedUnbind();
}

VertexBuffer::VertexBuffer(VertexBuffer::Type _type, VertexBuffer::Usage _usage): 
    type(_type),
    usage(_usage)
{
    _UpdateCachedValue();
}

} // namespace gldr
