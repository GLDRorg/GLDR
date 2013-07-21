#pragma once
#include "Bindable.hpp"

namespace gldr {

class Sampler {
    Glid<Sampler> id;

public:
    static GLuint create() {
        GLuint id = 0;
        gl::GenSamplers(1, &id);
        if (!id) {
            throw std::runtime_error("Problem creating a sampler");
        }
        return id;
    }
    static void destroy(GLuint id) {
        gl::DeleteSamplers(1, &id);
    }

    void bindToUnit(unsigned textureUnit) {
        gl::BindSampler(textureUnit, id.get());
    }

    void samplerParameter(GLenum pname, int param) {
        gl::SamplerParameteri(id.get(), pname, param);
    }
    void samplerParameter(GLenum pname, float param) {
        gl::SamplerParameterf(id.get(), pname, param);
    }
};

}
