#pragma once
#include "glid.hpp"
namespace gldr{
    struct VertexBuffer{
        enum class Type : GLuint {
            DATA  = gl::GL_ARRAY_BUFFER,
            INDEX = gl::GL_ELEMENT_ARRAY_BUFFER
        };

        enum class Usage : GLuint {
            STATIC_DRAW  = gl::GL_STATIC_DRAW,
            DYNAMIC_DRAW = gl::GL_DYNAMIC_DRAW
            /* STATIC_READ, STATIC_WRITE, DYNAMIC_READ, DYNAMIC_WRITE*/
        };

        VertexBuffer(Type type = Type::DATA, Usage usage = Usage::STATIC_DRAW):
            type(type), usage(usage){}

        template <typename T>
        void bufferData(std::vector<T> data){
            if(vboID){
                bind();
                gl::BufferData(static_cast<GLuint>(type), sizeof(T) * data.size(), data.data(), static_cast<GLuint>(usage));
            }
        }

        template <typename T>
        void bufferSubData(std::vector<T> data, GLintptr offSet){
            if(vboID){
                bind();
                gl::BufferSubData(static_cast<GLuint>(type), offSet, sizeof(T) * data.size(), data.data());
            }
        }

        void bind() const{
            if(vboID){
                gl::BindBuffer(static_cast<GLuint>(type), vboID);
            }
        }

        static GLuint creater(){
            GLuint id;
            gl::GenBuffers(1, &id);
            return id; 
        }        

        static void deleter(GLuint& id){
            gl::DeleteBuffers(1, &id);
        }

    private:
        Glid<VertexBuffer> vboID;
        Type type;
        Usage usage;
    };
}