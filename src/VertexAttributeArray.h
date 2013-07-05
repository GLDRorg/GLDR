#pragma once
#include <utility>
#include <stdexcept>
#include "GLId.h"

namespace gldr {

namespace {
    GLuint vaoCreate () { 
        GLuint id = 0;
        glGenVertexArrays(1, &id);
        if (!id)
            throw std::runtime_error("Problem creating a texture");
        return id;
    }

    void vaoDelete(GLuint id) {
        glDeleteVertexArrays(1, &id);
    }

    class VaoGLId : public GLId {
        VaoGLId(const VaoGLId& other) /* = delete */;
    public:
        VaoGLId(VaoGLId&& other) 
            : GLId(std::move(other)) {
        }
        VaoGLId()
            : GLId(vaoCreate(), vaoDelete)
        { }
        VaoGLId(GLuint id)
            : GLId(id, vaoDelete)
        { }
    };
    // hmm
    //typedef GLId<decltype(&detail::vaoDelete)> VaoGLId ;
}

class VertexAttributeArray {
private:
    VaoGLId id;

public:
    void bind() {
        glBindVertexArray(id);
    }
    void draw(int startIx, int endIx) {
        bind();
        glDrawArrays(GL_TRIANGLES, startIx, endIx);
    }
    void VertexAttributeArray::enableAttributeArray(unsigned index) {
        bind();
        glEnableVertexAttribArray(index);
    }

    static unsigned VertexAttributeArray::GetMaxVertexAttributes() {
        GLint count;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &count);
        return count;
    }

    VertexAttributeArray(VertexAttributeArray&& other) :
        id(std::move(other.id))
    { }
};

} // namespace gldr