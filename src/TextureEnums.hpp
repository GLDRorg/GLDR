#pragma once
#include <GL/glew.h>

namespace gldr { namespace texture_desc {

enum class Type : GLenum {
    Texture_1D = GL_TEXTURE_1D,
    Texture_2D = GL_TEXTURE_2D,
    Texture_3D = GL_TEXTURE_3D
};

enum class FilteringDirection : GLenum {
    Minification = GL_TEXTURE_MIN_FILTER,
    Magnification = GL_TEXTURE_MAG_FILTER
};

enum class FilteringMode : GLint {
    Linear = GL_LINEAR,
    LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
    LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
    Nearest = GL_NEAREST,
    NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
    NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST
};

enum class InternalFormat : GLenum {
    RGBA = GL_RGBA,
    RGB = GL_RGB,
    SRGB = GL_SRGB8,
    SRGBA = GL_SRGB8_ALPHA8,
    Depth = GL_DEPTH_COMPONENT
};

enum class Format : GLenum {
    RGBA = GL_RGBA,
    RGB = GL_RGB,
    Depth = GL_DEPTH_COMPONENT,
    BGRA = GL_BGRA,
    BGR = GL_BGR
};

enum class DataType : GLenum {
    Float = GL_FLOAT,
    UnsignedByte = GL_UNSIGNED_BYTE
};

} // desc
} // engine