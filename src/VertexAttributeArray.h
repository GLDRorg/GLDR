#pragma once
#include <utility>
#include <stdexcept>

#include "Bindable.hpp"
#include "VertexBuffer.h"
#include "Util.hpp"

#define GLDR_HAS_DSA

namespace gldr {

enum class VertexAttributeType : GLenum {
    Float = gl::FLOAT,
    HalfFloat = gl::HALF_FLOAT,
    Byte = gl::BYTE,
    Short = gl::SHORT,
    UShort = gl::UNSIGNED_SHORT,
    Int = gl::INT,
    UInt = gl::UNSIGNED_INT,
    UByte = gl::UNSIGNED_BYTE
};

enum class VertexAttributeIntegerType : GLenum {
    Byte = gl::BYTE,
    Short = gl::SHORT,
    UShort = gl::UNSIGNED_SHORT,
    Int = gl::INT,
    UInt = gl::UNSIGNED_INT,
    UByte = gl::UNSIGNED_BYTE
};

class VertexAttributeArray : public Bindable<VertexAttributeArray> {

public:
    // creation and destruction
    static GLuint create() {
        GLuint id = 0;
        gl::GenVertexArrays(1, &id);
        if (!id)
            throw std::runtime_error("Problem creating a vertex array");
        return id;
    }

    static void destroy(GLuint id) {
        gl::DeleteVertexArrays(1, &id);
    }

    static void bindObject(GLuint id) {
        gl::BindVertexArray(id);
    }

    static GLuint getCurrentlyBound() {
        GLint current;
        gl::GetIntegerv(gl::VERTEX_ARRAY_BINDING, &current);
        return current;
    }

    void directVertexAttribOffset(VertexBuffer<>& vbo, unsigned index, int size, VertexAttributeType type, bool normalized, unsigned stride, int offset) {
        directVertexAttribOffset(vbo.id.get(), index, size, type, normalized, stride, offset);
    }
    void directVertexAttribOffset(GLuint buffer, unsigned index, int size, VertexAttributeType type, bool normalized, unsigned stride, int offset) {
    #ifdef GLDR_HAS_DSA
        gl::VertexArrayVertexAttribOffsetEXT(id.get(), buffer, index, size, static_cast<GLenum>(type), normalized, stride, offset);
    #else
        auto vaoScope = scopedBind();
        auto vboScope = VertexBuffer<VertexBufferType::ARRAY_BUFFER>::createRebindToCurrent();

        gl::BindBuffer(gl::ARRAY_BUFFER, buffer);
        gl::VertexAttribPointer(index, size, static_cast<GLenum>(type), normalized, stride, reinterpret_cast<void*>(offset));
    #endif
    }

    void directVertexAttribIntegerOffset(VertexBuffer<>& vbo, unsigned index, int size, VertexAttributeIntegerType type, unsigned stride, int offset) {
        directVertexAttribIntegerOffset(vbo.id.get(), index, size, type, stride, offset);
    }
    void directVertexAttribIntegerOffset(GLuint buffer, unsigned index, int size, VertexAttributeIntegerType type, unsigned stride, int offset) {
    #ifdef GLDR_HAS_DSA
        gl::VertexArrayVertexAttribIOffsetEXT(id.get(), buffer, index, size, static_cast<GLenum>(type), stride, offset);
    #else
        auto vaoScope = scopedBind();
        auto vboScope = VertexBuffer<VertexBufferType::ARRAY_BUFFER>::createRebindToCurrent();

        gl::BindBuffer(gl::ARRAY_BUFFER, buffer);
        gl::VertexAttribIPointer(index, size, static_cast<GLenum>(type), stride, reinterpret_cast<void*>(offset));
    #endif
    }

    void enableAttributeArray(unsigned index) {
    #ifdef GLDR_HAS_DSA
        gl::EnableVertexArrayAttribEXT(id.get(), index);
    #else
        auto scope = scopedBind();
        gl::EnableVertexAttribArray(index);
    #endif
    }

    // static state queries
    // TODO: MAX_VERTEX_ATTRIB_BINDINGS is a part of buffer specification, not VAO?
    // Spec 4.3, 10.3.1-10.3.3
    // TODO: is this too much of utility?
    static unsigned getMaxVertexAttributes() {
        return util::getState(gl::MAX_VERTEX_ATTRIBS);
    }

    VertexAttributeArray()
    { }
    VertexAttributeArray(VertexAttributeArray && other)
        : Bindable(std::move(other))
    { }
    VertexAttributeArray& operator=(VertexAttributeArray && other) {
        Bindable::operator=(std::move(other));
    }
};

} // namespace gldr