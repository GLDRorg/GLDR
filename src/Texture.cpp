#include "texture.hpp"

namespace gldr {

template<>
void Texture<texture_desc::Type::Texture_1D>::imageData(
    unsigned width,
    unsigned height,
    texture_desc::Format format,
    texture_desc::InternalFormat internalFormat,
    texture_desc::DataType dataType,
    const void* data)
{
    lastFormat = format;
    gl::TextureImage1DEXT(id.get(), 
        gl::TEXTURE_1D,
        0,
        static_cast<GLint>(internalFormat),
        width,
        height,
        0,
        static_cast<GLenum>(format),
        data);
}

template<>
void Texture<texture_desc::Type::Texture_2D>::imageData(
    unsigned width,
    unsigned height,
    texture_desc::Format format,
    texture_desc::InternalFormat internalFormat,
    texture_desc::DataType dataType,
    const void* data)
{
    lastFormat = format;
    gl::TextureImage2DEXT(id.get(),
        gl::TEXTURE_2D,
        0,
        static_cast<GLint>(internalFormat),
        width,
        height,
        0,
        static_cast<GLenum>(format),
        static_cast<GLenum>(dataType),
        data);
}

template<>
void Texture<texture_desc::Type::Texture_3D>::imageData(
    unsigned width,
    unsigned height,
    texture_desc::Format format,
    texture_desc::InternalFormat internalFormat,
    texture_desc::DataType dataType,
    const void* data)
{
    lastFormat = format;
    gl::TextureImage3DEXT(id.get(),
        gl::TEXTURE_3D,
        0,
        static_cast<GLint>(internalFormat),
        width,
        height,
        0,
        0,
        static_cast<GLenum>(format),
        static_cast<GLenum>(dataType),
        data);
}

}
