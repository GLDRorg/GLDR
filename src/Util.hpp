#pragma once
#include <glid.hpp>
#include <stdexcept>
#include <vector>

namespace gldr { namespace util {

    inline void glShaderSource_gldr(GLuint shader, std::string const& shaderSource)
    {
        if (shaderSource.empty())
            throw std::runtime_error("Empty shader passed to `glShaderSource`");
        const GLchar* ptr = shaderSource.c_str();
        const GLint size = shaderSource.size();
        gl::ShaderSource(shader, 1, &ptr, &size);
    }

    inline void glShaderSource_gldr(GLuint shader, std::vector<char> const& shaderSource)
    {
        if (shaderSource.empty())
            throw std::runtime_error("Empty shader passed to `glShaderSource`");
        const GLchar* ptr = &shaderSource[0];
        const GLint size = shaderSource.size();
        gl::ShaderSource(shader, 1, &ptr, &size);
    }

    inline GLint getState(GLenum pname) {
        GLint value;
        gl::GetIntegerv(pname, &value);
        return value;
    }

    typedef std::add_pointer<void(GLenum, GLint*)>::type GetPtr_t;
    template<GetPtr_t fun>
    int getState_DONTUSEYET(GLenum pname) {
        GLint value;
        fun(pname, &value);
        return value;
    }

}
}