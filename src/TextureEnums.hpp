#pragma once

namespace gldr { namespace texture_desc {

enum class Type : GLenum {
    Texture_1D = gl::TEXTURE_1D,
    Texture_2D = gl::TEXTURE_2D,
    Texture_3D = gl::TEXTURE_3D
};

enum class FilteringDirection : GLenum {
    Minification = gl::TEXTURE_MIN_FILTER,
    Magnification = gl::TEXTURE_MAG_FILTER
};

enum class FilteringMode : GLint {
    Linear = gl::LINEAR,
    LinearMipmapLinear = gl::LINEAR_MIPMAP_LINEAR,
    LinearMipmapNearest = gl::LINEAR_MIPMAP_NEAREST,
    Nearest = gl::NEAREST,
    NearestMipmapLinear = gl::NEAREST_MIPMAP_LINEAR,
    NearestMipmapNearest = gl::NEAREST_MIPMAP_NEAREST
};

enum class InternalFormat : GLenum {
    RGBA = gl::RGBA,
    RGB = gl::RGB,
    RGB8 = gl::RGB8,
    SRGB = gl::SRGB8,
    SRGBA = gl::SRGB8_ALPHA8,
    Depth = gl::DEPTH_COMPONENT
};

enum class Format : GLenum {
    RGBA = gl::RGBA,
    RGB = gl::RGB,
    Depth = gl::DEPTH_COMPONENT,
    BGRA = gl::BGRA,
    BGR = gl::BGR
};

enum class DataType : GLenum {
    Float = gl::FLOAT,
    UnsignedByte = gl::UNSIGNED_BYTE
};

} // desc
} // engine