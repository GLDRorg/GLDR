#pragma once
namespace gldr {
    namespace textureOptions {
        enum class Dimension : GLenum {
            Texture_1D = gl::GL_TEXTURE_1D,
            Texture_2D = gl::GL_TEXTURE_2D,
            Texture_3D = gl::GL_TEXTURE_3D,
        };
        
        enum class FilterDirection : GLenum {
            Minification  = gl::GL_TEXTURE_MIN_FILTER,
            Magnification = gl::GL_TEXTURE_MAG_FILTER,
        };
        
        enum class FilterMode : GLint {
            Linear               = gl::GL_LINEAR,
            LinearMipmapLinear   = gl::GL_LINEAR_MIPMAP_LINEAR,
            LinearMipmapNearest  = gl::GL_LINEAR_MIPMAP_NEAREST,
            Nearest              = gl::GL_NEAREST,
            NearestMipmapLinear  = gl::GL_NEAREST_MIPMAP_LINEAR,
            NearestMipmapNearest = gl::GL_NEAREST_MIPMAP_NEAREST,
        };

        enum class WrapDirection : GLenum {
            S = gl::GL_TEXTURE_WRAP_S,
            T = gl::GL_TEXTURE_WRAP_T
        };
        
        enum class WrapMode : GLint {
            EdgeClamp = gl::GL_CLAMP_TO_EDGE,
            BoderClamp = gl::GL_CLAMP_TO_BORDER,
            Mirrored = gl::GL_MIRRORED_REPEAT,
            Repeat = gl::GL_REPEAT,
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
	}
}