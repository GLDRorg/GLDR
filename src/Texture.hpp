#pragma once

#include <stdexcept>

#include "TextureEnums.hpp"
#include "Bindable.hpp"

#include <utility>

#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/cat.hpp>

#define GLDR_HAS_DSA

namespace gldr {

template<texture_desc::Type type>
class Texture {
    texture_desc::Format lastFormat;
    Glid<Texture> id;
public:
    friend class Framebuffer;

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

    /*static void bindObject(GLuint id) {
        //TEMP
        int textureUnit = 0;
    #ifdef GLDR_HAS_DSA
        gl::BindMultiTextureEXT(gl::TEXTURE0 + textureUnit, static_cast<GLenum>(type), id);
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
    }*/

    void bindToUnit(unsigned textureUnit) {
        gl::BindMultiTextureEXT(gl::TEXTURE0 + textureUnit, static_cast<GLenum>(type), id.get());
    }
    
    template<typename value_t>
    void setParameter (GLenum pname, value_t value) {
    #ifdef GLDR_HAS_DSA
        gl::TextureParameteriEXT(id.get(), static_cast<GLenum>(type), pname, static_cast<GLint>(value));
    #else
        auto scope = scopedBind();
        gl::TexParameteri(static_cast<GLenum>(type), static_cast<GLenum>(pname), static_cast<GLint>(value));
    #endif
    }

    #define ADD_SET_TEX_PARAMETER(name, pname, value_t) \
    void BOOST_PP_CAT(set, name) (value_t value) { \
        setParameter<value_t>(pname, value); \
    }

    ADD_SET_TEX_PARAMETER(MagFiltering, gl::TEXTURE_MAG_FILTER, texture_desc::MagFilteringMode);
    ADD_SET_TEX_PARAMETER(MinFiltering, gl::TEXTURE_MIN_FILTER, texture_desc::MinFilteringMode);
    ADD_SET_TEX_PARAMETER(MinLOD, gl::TEXTURE_MIN_LOD, float);
    ADD_SET_TEX_PARAMETER(MaxLOD, gl::TEXTURE_MAX_LOD, float);
    #undef ADD_SET_TEX_PARAMETER

    //void setParameter(texture_desc::FilteringDirection direction, texture_desc::FilteringMode mode


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