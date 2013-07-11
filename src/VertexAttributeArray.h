#pragma once
#include <utility>
#include <stdexcept>

#include "GLId.h"

namespace gldr {

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

    /*void draw(int startIx, int endIx) {
        bind();
        glDrawArrays(GL_TRIANGLES, startIx, endIx);
    }*/

    void VertexAttributeArray::enableAttributeArray(unsigned index) {
        bind();
        gl::EnableVertexAttribArray(index);
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

    static GLuint getCurrentlyBound() {
        GLint current;
        gl::GetIntegerv(gl::VERTEX_ARRAY_BINDING, &current);
        return current;
    }
};

} // namespace gldr