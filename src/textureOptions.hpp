#pragma once
namespace gldr {
namespace textureOptions {
    enum class Dimension : GLenum {
        Texture_1D = gl::TEXTURE_1D,
        Texture_2D = gl::TEXTURE_2D,
        Texture_3D = gl::TEXTURE_3D,
        Texture_Cube = gl::TEXTURE_CUBE_MAP,
    };
    
    enum class FilterDirection : GLenum {
        Minification  = gl::TEXTURE_MIN_FILTER,
        Magnification = gl::TEXTURE_MAG_FILTER,
    };
    
    enum class FilterMode : GLint {
        Linear               = gl::LINEAR,
        LinearMipmapLinear   = gl::LINEAR_MIPMAP_LINEAR,
        LinearMipmapNearest  = gl::LINEAR_MIPMAP_NEAREST,
        Nearest              = gl::NEAREST,
        NearestMipmapLinear  = gl::NEAREST_MIPMAP_LINEAR,
        NearestMipmapNearest = gl::NEAREST_MIPMAP_NEAREST,
    };

    enum class WrapDirection : GLenum {
        S = gl::TEXTURE_WRAP_S,
        T = gl::TEXTURE_WRAP_T,
        R = gl::TEXTURE_WRAP_R
    };
    
    enum class WrapMode : GLint {
        EdgeClamp = gl::CLAMP_TO_EDGE,
        BoderClamp = gl::CLAMP_TO_BORDER,
        Mirrored = gl::MIRRORED_REPEAT,
        Repeat = gl::REPEAT,
    };

    enum class InternalFormat : GLenum {
        Red       = gl::RED,
        GreyScale = gl::RED,
        RGB       = gl::RGB,
        RGBA      = gl::RGBA,
        SRGB      = gl::SRGB8,
        SRGBA     = gl::SRGB8_ALPHA8,
        Depth     = gl::DEPTH_COMPONENT,
    };
    
    enum class Format : GLenum {
        Red       = gl::RED,
        GreyScale = gl::RED,
        RGB       = gl::RGB,
        RGBA      = gl::RGBA,
        BGR       = gl::BGR,
        BGRA      = gl::BGRA,
        Depth     = gl::DEPTH_COMPONENT,
    };
    
    enum class DataType : GLenum {
        Float = gl::FLOAT,
        UnsignedByte = gl::UNSIGNED_BYTE,
    };

    enum class CubeMapFace : GLenum {
        PositiveX = gl::TEXTURE_CUBE_MAP_POSITIVE_X,
        NegativeX = gl::TEXTURE_CUBE_MAP_NEGATIVE_X,
        PositiveY = gl::TEXTURE_CUBE_MAP_POSITIVE_Y,
        NegativeY = gl::TEXTURE_CUBE_MAP_NEGATIVE_Y,
        PositiveZ = gl::TEXTURE_CUBE_MAP_POSITIVE_Z,
        NegativeZ = gl::TEXTURE_CUBE_MAP_NEGATIVE_Z,
    };
}}