#pragma once

namespace gldr { namespace texture_desc {

enum class Type : GLenum {
    Texture_1D = gl::TEXTURE_1D,
    Texture_2D = gl::TEXTURE_2D,
    Texture_3D = gl::TEXTURE_3D,
    Texture_1DArray = gl::TEXTURE_1D_ARRAY,
    Texture_2DArray = gl::TEXTURE_2D_ARRAY,
    Rectangle = gl::TEXTURE_RECTANGLE,
    CubeMap = gl::TEXTURE_CUBE_MAP
};

enum class ParameterType : GLenum {
    DepthStencilMode = gl::DEPTH_STENCIL_TEXTURE_MODE,
    BaseLevel = gl::TEXTURE_BASE_LEVEL,
    CompareFunc = gl::TEXTURE_COMPARE_FUNC,
    CompareMode = gl::TEXTURE_COMPARE_MODE,
    LODBias = gl::TEXTURE_LOD_BIAS,
    MinificationFilter = gl::TEXTURE_MIN_FILTER,
    MagnificationFilter = gl::TEXTURE_MAG_FILTER,
    MinimalLOD = gl::TEXTURE_MIN_LOD,
    MaximalLOD = gl::TEXTURE_MAX_LOD,
    MaximalLevel = gl::TEXTURE_MAX_LEVEL,
    SwizzleR = gl::TEXTURE_SWIZZLE_R,
    SwizzleG = gl::TEXTURE_SWIZZLE_G,
    SwizzleB = gl::TEXTURE_SWIZZLE_B,
    SwizzleA = gl::TEXTURE_SWIZZLE_A,
    WrapS = gl::TEXTURE_WRAP_S,
    WrapT = gl::TEXTURE_WRAP_T,
    WrapR = gl::TEXTURE_WRAP_R
};

enum class MinFilteringMode : GLint {
    Linear = gl::LINEAR,
    LinearMipmapLinear = gl::LINEAR_MIPMAP_LINEAR,
    LinearMipmapNearest = gl::LINEAR_MIPMAP_NEAREST,
    Nearest = gl::NEAREST,
    NearestMipmapLinear = gl::NEAREST_MIPMAP_LINEAR,
    NearestMipmapNearest = gl::NEAREST_MIPMAP_NEAREST
};

enum class MagFilteringMode : GLint {
    Linear = gl::LINEAR,
    Nearest = gl::NEAREST
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