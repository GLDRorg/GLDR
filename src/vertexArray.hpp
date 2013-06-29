#pragma once
#include "glid.hpp"
namespace gldr{
    struct VertexArray{
        VertexArray(){
            gl::GenVertexArrays(1, vaoID.ptr());
        }

        void bind() const{
            if(vaoID){
                gl::BindVertexArray(vaoID);
            }
        }

        static void deletor(GLuint& id){
            gl::DeleteVertexArrays(1, &id);
        }
    private:
        Glid<deletor> vaoID;
    };
}