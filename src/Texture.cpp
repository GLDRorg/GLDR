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
    bind();
    glTexImage1D(GL_TEXTURE_1D,
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
    bind();
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        static_cast<GLint>(internalFormat),
        width,
        height,
        0,
        static_cast<GLenum>(format),
        GL_UNSIGNED_BYTE,
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
    bind();
    glTexImage3D(
        GL_TEXTURE_3D,
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
