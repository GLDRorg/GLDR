#pragma once
#include <utility>
#include <stdexcept>

#include "GLId.h"
#include "Bindable.hpp"
#include "VertexBuffer.h"

//#define GLDR_HAS_DSA

namespace gldr {

enum class VertexAttributeType : GLenum {
    Float = gl::FLOAT,
    HalfFloat = gl::HALF_FLOAT,
    UByte = gl::UNSIGNED_BYTE
};

class VertexAttributeArray : public Bindable<VertexAttributeArray> {
    //Glid<VertexAttributeArray> id;

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

    static void bindObject(GLuint id) {
        gl::BindVertexArray(id);
    }

    static GLuint getCurrentlyBound() {
        GLint current;
        gl::GetIntegerv(gl::VERTEX_ARRAY_BINDING, &current);
        return current;
    }

    /*void checkedBind() {
        if ((savedId = getCurrentlyBound()) != id.get()) {
            bind();
            // this is here because of "don't pay for what you don't use"
            // involved in forceBind
            savedId = id.get();
        }
    }*/

    /*void bind() {
        gl::BindVertexArray(id.get());
    }*/

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
        auto vaoScope = scopedBind();
        auto vboScope = VertexBuffer<VertexBufferType::ARRAY_BUFFER>::createRebindToCurrent();

        gl::BindBuffer(gl::ARRAY_BUFFER, buffer);
        gl::VertexAttribPointer(index, size, static_cast<GLenum>(type), normalized, stride, reinterpret_cast<void*>(offset));
    #endif
    }

    void vertexAttribIntegerOffset(unsigned buffer, unsigned index, int size, VertexAttributeType type, unsigned stride, int offset) {
    #ifdef GLDR_HAS_DSA
        gl::VertexArrayVertexAttribIOffsetEXT(id.get(), buffer, index, size, static_cast<GLenum>(type), stride, offset);
    #else
    //    auto scope = scopedBind();
    //    gl::VertexAttribIPointer();
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
        auto scope = scopedBind();
        gl::EnableVertexAttribArray(index);
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