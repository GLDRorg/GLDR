#pragma once

#include <stdexcept>

#include "TextureEnums.hpp"
#include "Bindable.hpp"

#include <utility>

//#define GLDR_HAS_DSA

namespace gldr {

template<texture_desc::Type type>
class Texture : public Bindable<Texture<type>> {
    texture_desc::Format lastFormat;

public:
    static GLuint create() {
        GLuint id = 0;
        gl::GenTextures(1, &id);
        if (!id)
            throw std::runtime_error("Problem creating a texture");
        return id;
    }

    static void destroy(GLuint id) {
        gl::DeleteTextures(1, &id);
    }

    static void bindObject(GLuint id) {
        //TEMP
        int textureUnit = 0;
    #ifdef GLDR_HAS_DSA
        gl::BindMultiTextureEXT(gl::TEXTURE0 + textureUnit, static_cast<GLenum>(type), id.get());
    #else
        //GLint active;
        //gl::GetIntegerv(gl::ACTIVE_TEXTURE, &active);

        gl::ActiveTexture(gl::TEXTURE0 + textureUnit);
        gl::BindTexture(static_cast<GLenum>(type), id);
        //auto scope = scopedBind();

        //gl::ActiveTexture(active);
    #endif
    }

    static GLuint getCurrentlyBound() {
        GLint current;
        switch (type) {
        case texture_desc::Type::Texture_1D:
            gl::GetIntegerv(gl::TEXTURE_BINDING_1D, &current);
            break;
        case texture_desc::Type::Texture_2D:
            gl::GetIntegerv(gl::TEXTURE_BINDING_2D, &current);
            break;
        case texture_desc::Type::Texture_3D:
            gl::GetIntegerv(gl::TEXTURE_BINDING_3D, &current);
            break;
        }
        return current;
    }

    void bind(unsigned textureUnit) {
        bindObject(id.get());
    }

    void setFiltering (texture_desc::FilteringDirection direction, texture_desc::FilteringMode mode) {
    #ifdef GLDR_HAS_DSA
        gl::TextureParameteriEXT(id.get(), static_cast<GLenum>(type), static_cast<GLenum>(direction), static_cast<GLint>(mode));
    #else
        auto scope = scopedBind();
        gl::TexParameteri(static_cast<GLenum>(type), static_cast<GLenum>(direction), static_cast<GLint>(mode));
    #endif
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

    Texture() { }

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