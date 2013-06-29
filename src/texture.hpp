#pragma once
#include "glid.hpp"
#include "textureOptions.hpp"
namespace gldr{
    namespace {
        void deleteTexture(GLuint& id){
            gl::DeleteTextures(1, &id);
        }
    }
    template <textureOptions::Dimension dimension>
    struct Texture{
        Texture() {
            gl::GenTextures(1, textureID.ptr());
        }

        void bind() {
            if(textureID){
                gl::BindTexture(static_cast<GLenum>(dimension), textureID);
            }
        }

        void bind(unsigned textureUnit) { // possibly go for boost::optional
            gl::ActiveTexture(gl::GL_TEXTURE0 + textureUnit);
            bind();
        }

        void setFiltering(textureOptions::FilterDirection direction, textureOptions::FilterMode mode) {
            if(textureID){
                bind();
                gl::TexParameteri(static_cast<GLenum>(dimension), static_cast<GLenum>(direction), static_cast<GLint>(mode));
            }
        }

        void setWrapMode(textureOptions::WrapDirection direction, textureOptions::WrapMode mode){
            if(textureID){
                bind();
                gl::TexParameteri(static_cast<GLenum>(dimension), static_cast<GLenum>(direction), static_cast<GLint>(mode));
            }
        }

        void imageData(unsigned width, unsigned height,
            textureOptions::Format format, textureOptions::InternalFormat internalFormat,
            textureOptions::DataType dataType, const void* data);

    private:
        Glid<deleteTexture> textureID;
    };

    template<>
    void Texture<textureOptions::Dimension::Texture_1D>::imageData(unsigned width, unsigned height,
                    textureOptions::Format format, textureOptions::InternalFormat internalFormat,
                    textureOptions::DataType dataType, const void* data){
        gl::TexImage1D(
            gl::GL_TEXTURE_1D, 0,
            static_cast<GLint>(internalFormat),
            width, height, 0,
            static_cast<GLenum>(format), data);
    }

    template<>
    void Texture<textureOptions::Dimension::Texture_2D>::imageData(unsigned width, unsigned height,
                    textureOptions::Format format, textureOptions::InternalFormat internalFormat,
                    textureOptions::DataType dataType, const void* data){
        gl::TexImage2D(
            gl::GL_TEXTURE_2D, 0,
            static_cast<GLint>(internalFormat),
            width, height, 0,
            static_cast<GLenum>(format), gl::GL_UNSIGNED_BYTE, data);
    }

    template<>    
    void Texture<textureOptions::Dimension::Texture_3D>::imageData(unsigned width, unsigned height,
                    textureOptions::Format format, textureOptions::InternalFormat internalFormat,
                    textureOptions::DataType dataType, const void* data){
        /*gl::TexImage3D(
            gl::GL_TEXTURE_3D, 0,
            static_cast<GLint>(internalFormat),
            width, height, 0, 0,
            static_cast<GLenum>(format),
            static_cast<GLenum>(dataType), data);*/
    }

    typedef Texture<textureOptions::Dimension::Texture_1D> Texture1d;
    typedef Texture<textureOptions::Dimension::Texture_2D> Texture2d;
    typedef Texture<textureOptions::Dimension::Texture_3D> Texture3d;
}