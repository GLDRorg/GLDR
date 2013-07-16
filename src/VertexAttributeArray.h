#pragma once
#include <utility>
#include <stdexcept>

#include "GLId.h"

#define GLDR_HAS_DSA

namespace gldr {

enum class VertexAttributeType : GLenum {
    Float = gl::FLOAT,
    HalfFloat = gl::HALF_FLOAT,
    UByte = gl::UNSIGNED_BYTE
};

class VertexAttributeArray {
    Glid<VertexAttributeArray> id;
    GLuint savedId;

public:
    // creation and destruction
    static GLuint create() {
        GLuint id = 0;
        gl::GenVertexArrays(1, &id);
        if (!id)
            throw std::runtime_error("Problem creating a texture");
        return id;
    }

    static void destroy(GLuint id) {
        gl::DeleteVertexArrays(1, &id);
    }

    static GLuint getCurrentlyBound() {
        GLint current;
        gl::GetIntegerv(gl::VERTEX_ARRAY_BINDING, &current);
        return current;
    }

    void checkedBind() {
        if ((savedId = getCurrentlyBound()) != id.get()) {
            bind();
            // this is here because of "don't pay for what you don't use"
            // involved in forceBind
            savedId = id.get();
        }
    }

    void bind() {
        gl::BindVertexArray(id.get());
    }

    void vertexAttribOffset(unsigned index, int size, VertexAttributeType type, bool normalized, unsigned stride, int offset) {
        auto current = getCurrentlyBound();
        bind();

        gl::VertexAttribPointer(index, size, static_cast<GLenum>(type), normalized, stride, reinterpret_cast<void*>(offset));

        gl::BindVertexArray(current);
    }

    void directVertexAttribOffset(unsigned buffer, unsigned index, int size, VertexAttributeType type, bool normalized, unsigned stride, int offset) {
    #ifdef GLDR_HAS_DSA
        gl::VertexArrayVertexAttribOffsetEXT(id.get(), buffer, index, size, static_cast<GLenum>(type), normalized, stride, offset);
    #else
        auto current = getCurrentlyBound();
        bind();

        GLint currentBuffer;
        gl::GetIntegerv(gl::ARRAY_BUFFER_BINDING, &currentBuffer);

        gl::BindBuffer(gl::ARRAY_BUFFER, buffer);
        gl::VertexAttribPointer(index, size, static_cast<GLenum>(type), normalized, stride, reinterpret_cast<void*>(offset));

        gl::BindBuffer(gl::ARRAY_BUFFER, currentBuffer);
        gl::BindVertexArray(current);
    #endif
    }

    void vertexAttribIntegerOffset(unsigned buffer, unsigned index, int size, VertexAttributeType type, unsigned stride, int offset) {
    #ifdef GLDR_HAS_DSA
        gl::VertexArrayVertexAttribIOffsetEXT(id.get(), buffer, index, size, static_cast<GLenum>(type), stride, offset);
    #else
        #error "That function is not implemented yet"
    #endif
    }

    /*void draw(int startIx, int endIx) {
        bind();
        glDrawArrays(GL_TRIANGLES, startIx, endIx);
    }*/

    void VertexAttributeArray::enableAttributeArray(unsigned index) {
    #ifdef GLDR_HAS_DSA
        gl::EnableVertexArrayAttribEXT(id.get(), index);
    #else
        auto current = getCurrentlyBound();
        bind();
        gl::EnableVertexAttribArray(index);
        gl::BindVertexArray(current);
    #endif
    }

    // static state queries
    // TODO: MAX_VERTEX_ATTRIB_BINDINGS is a part of buffer specification, not VAO?
    // Spec 4.3, 10.3.1-10.3.3
    // TODO: is this too much of utility?
    static unsigned getMaxVertexAttributes() {
        GLint count;
        gl::GetIntegerv(gl::MAX_VERTEX_ATTRIBS, &count);
        return count;
    }
};

} // namespace gldr