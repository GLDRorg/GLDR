#pragma once
#include "glid.hpp"
#include "textureOptions.hpp"
namespace gldr{
struct TextureCube{
    void bind() const{
        if(textureID.get()){
            gl::BindTexture(static_cast<GLenum>(textureOptions::Dimension::Texture_Cube), textureID.get());
        }
    }

    // void bind(unsigned textureUnit) const{ // possibly go for boost::optional
    //     gl::ActiveTexture(gl::TEXTURE0 + textureUnit);
    //     bind();
    // }

    void setFiltering(textureOptions::FilterDirection direction, textureOptions::FilterMode mode){
        if(textureID.get()){
            bind();
            gl::TexParameteri(gl::TEXTURE_CUBE_MAP, static_cast<GLenum>(direction), static_cast<GLint>(mode));
        }
    }

    void setWrapMode(textureOptions::WrapDirection direction, textureOptions::WrapMode mode){
        if(textureID.get()){
            bind();
            gl::TexParameteri(gl::TEXTURE_CUBE_MAP, static_cast<GLenum>(direction), static_cast<GLint>(mode));
        }
    }

    inline void imageData(textureOptions::CubeMapFace face, unsigned width, unsigned height,
        textureOptions::Format format, textureOptions::InternalFormat internalFormat,
        textureOptions::DataType dataType, const void* data){
        gl::TexImage2D(
            static_cast<GLenum>(face), 0,
            static_cast<GLint>(internalFormat),
            width, height, 0,
            static_cast<GLenum>(format), gl::UNSIGNED_BYTE, data);
    }


    static GLuint create(){
        GLuint id;
        gl::GenTextures(1, &id);
        return id; 
    }        

    static void destroy(GLuint& id){
        gl::DeleteTextures(1, &id);
    }

private:
    Glid<TextureCube> textureID;
};
}