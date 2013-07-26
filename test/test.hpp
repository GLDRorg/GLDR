#pragma once

#include <gldr.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ProgramGLM : public gldr::Program {
    ProgramGLM(ProgramGLM const&) /*= delete*/;
    ProgramGLM& operator=(ProgramGLM const&) /*= delete*/;
public:
    ProgramGLM(){ }

    void setUniform(std::string const& name, glm::vec2 const& vec) {
        GLint Loc = getUniformLocation(name);
        if (Loc != -1) {
            gl::ProgramUniform2fv(id.get(), Loc, 1, glm::value_ptr(vec));
        }
    }

    void setUniform(std::string const& name, glm::vec3 const& vec) {
        GLint Loc = getUniformLocation(name);
        if (Loc != -1) {
            bind();
            gl::ProgramUniform3fv(id.get(), Loc, 1, glm::value_ptr(vec));
        }
    }

    void setUniform(std::string const& name, glm::mat4 const& mat) {
        GLint Loc = getUniformLocation(name);
        if (Loc != -1) {
            gl::ProgramUniformMatrix4fv(id.get(), Loc, 1, gl::FALSE_, glm::value_ptr(mat));
        }
    }

    void setUniform(std::string const& name, glm::vec4 const& color) {
        GLint Loc = getUniformLocation(name);
        if (Loc != -1) {
            gl::ProgramUniform4fv(id.get(), Loc, 1, glm::value_ptr(color));
        }
    }

    void setUniform(std::string const& name, std::vector<glm::vec4> vecs) {
        GLint loc = getUniformLocation(name);
        if (loc != -1) {
            gl::ProgramUniform4fv(id.get(), loc, vecs.size(), reinterpret_cast<GLfloat*>(vecs.data()));
        }
    }
};

class TestBase {
public:
    virtual void render() = 0;
};
