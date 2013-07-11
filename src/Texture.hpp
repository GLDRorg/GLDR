#pragma once

#include <stdexcept>

#include "TextureEnums.hpp"
#include "GLId.h"

#include <utility>

namespace gldr {

namespace detail {
    GLuint textureCreate () { 
        GLuint id = 0;
        glGenTextures(1, &id);
        if (!id)
            throw std::runtime_error("Problem creating a texture");
        return id;
    }

    void textureDelete(GLuint id) {
        glDeleteTextures(1, &id);
    }
}

template<texture_desc::Type type>
class Texture {
    GLId id;
    texture_desc::Format lastFormat;

public:
    void bind() {
        glBindTexture(static_cast<GLenum>(type), id);
    }

    void bind(unsigned textureUnit) {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        bind();
    }

    void setFiltering (texture_desc::FilteringDirection direction, texture_desc::FilteringMode mode) {
        bind();
        glTexParameteri(static_cast<GLenum>(type), static_cast<GLenum>(direction), static_cast<GLint>(mode));
    }

    inline void imageData(unsigned width,
                   unsigned height,
                   texture_desc::Format format,
                   texture_desc::InternalFormat internalFormat,
                   texture_desc::DataType dataType,
                   const void* data);

    // to discuss further
    //GLuint getId() const { return id; }
    texture_desc::Format getFormat() const { return lastFormat; }

    Texture () : id(detail::textureCreate(), detail::textureDelete) {
    }

    Texture(Texture&& other) 
        : id(std::move(other.id)) {
    }

    Texture& operator= (Texture&& other) {
        id = std::move(other.id);
        return *this;
    }

private:
    Texture (Texture const&)/* = delete*/;
    Texture& operator= (Texture const&)/* = delete*/;
};

typedef Texture<texture_desc::Type::Texture_1D> Texture1d;
typedef Texture<texture_desc::Type::Texture_2D> Texture2d;
typedef Texture<texture_desc::Type::Texture_3D> Texture3d;

}

#include "Texture.cpp"