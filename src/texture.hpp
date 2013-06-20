#pragma once
#include "glid.hpp"
namespace gldr{
    struct Texture{
        enum class Dimension : GLenum {
            Texture_1D = gl::GL_TEXTURE_1D,
            Texture_2D = gl::GL_TEXTURE_2D,
            Texture_3D = gl::GL_TEXTURE_3D,
        };
        
        enum class FilteringDirection : GLenum {
            Minification  = gl::GL_TEXTURE_MIN_FILTER,
            Magnification = gl::GL_TEXTURE_MAG_FILTER,
        };
        
        enum class FilteringMode : GLint {
            Linear               = gl::GL_LINEAR,
            LinearMipmapLinear   = gl::GL_LINEAR_MIPMAP_LINEAR,
            LinearMipmapNearest  = gl::GL_LINEAR_MIPMAP_NEAREST,
            Nearest              = gl::GL_NEAREST,
            NearestMipmapLinear  = gl::GL_NEAREST_MIPMAP_LINEAR,
            NearestMipmapNearest = gl::GL_NEAREST_MIPMAP_NEAREST,
        };
        
        enum class InternalFormat : GLenum {
            RGB   = gl::GL_RGB,
            RGBA  = gl::GL_RGBA,
            SRGB  = gl::GL_SRGB8,
            SRGBA = gl::GL_SRGB8_ALPHA8,
            Depth = gl::GL_DEPTH_COMPONENT,
        };
        
        enum class Format : GLenum {
            RGB   = gl::GL_RGB,
            RGBA  = gl::GL_RGBA,
            BGR   = gl::GL_BGR,
            BGRA  = gl::GL_BGRA,
            Depth = gl::GL_DEPTH_COMPONENT,
        };
        
        enum class DataType : GLenum {
            Float = gl::GL_FLOAT,
            UnsignedByte = gl::GL_UNSIGNED_BYTE,
        };

        Texture(Dimension dimension = Dimension::Texture_2D):
            dimension(dimension),
            texture(&gl::DeleteTextures)
            {
            gl::GenTextures(1, texture.ptr());
        }

        void bind() {
            if(texture){
                gl::BindTexture(static_cast<GLenum>(dimension), texture);
            }
        }

        void bind(unsigned textureUnit) { // possibly go for boost::optional
            gl::ActiveTexture(gl::GL_TEXTURE0 + textureUnit);
            bind();
        }

        void setFiltering(FilteringDirection direction, FilteringMode mode) {
            if(texture){
                bind();
                gl::TexParameteri(static_cast<GLenum>(dimension), static_cast<GLenum>(direction), static_cast<GLint>(mode));
            }
        }

        void imageData(unsigned width, unsigned height,
                        Format format, InternalFormat internalFormat,
                        DataType dataType, const void* data){
            bind();
            switch(dimension){
                case Dimension::Texture_1D: imageData1D(width, height, format, internalFormat, dataType, data); break;
                case Dimension::Texture_2D: imageData2D(width, height, format, internalFormat, dataType, data); break;
                case Dimension::Texture_3D: imageData3D(width, height, format, internalFormat, dataType, data); break;
            }
        }

    private:
        void imageData1D(unsigned width, unsigned height,
                        Format format, InternalFormat internalFormat,
                        DataType dataType, const void* data){
            gl::TexImage1D(
                gl::GL_TEXTURE_1D, 0,
                static_cast<GLint>(internalFormat),
                width, height, 0,
                static_cast<GLenum>(format), data);
        }

        void imageData2D(unsigned width, unsigned height,
                        Format format, InternalFormat internalFormat,
                        DataType dataType, const void* data){
            gl::TexImage2D(
                gl::GL_TEXTURE_2D, 0,
                static_cast<GLint>(internalFormat),
                width, height, 0,
                static_cast<GLenum>(format), gl::GL_UNSIGNED_BYTE, data);
        }

        void imageData3D(unsigned width, unsigned height,
                        Format format, InternalFormat internalFormat,
                        DataType dataType, const void* data){
            /*gl::TexImage3D(
                gl::GL_TEXTURE_3D, 0,
                static_cast<GLint>(internalFormat),
                width, height, 0, 0,
                static_cast<GLenum>(format),
                static_cast<GLenum>(dataType), data);*/
        }

        Dimension dimension;
        Glid<decltype(&gl::DeleteTextures)> texture;
    };
}