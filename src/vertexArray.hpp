#pragma once
#include "glid.hpp"
namespace gldr{
    namespace {
        void deleteVertexArray(GLuint& id){
            gl::DeleteVertexArrays(1, &id);
        }
    }
    struct VertexArray{
        VertexArray(){
            gl::GenVertexArrays(1, vaoID.ptr());
        }

        void bind() const {
            if(vaoID){
                gl::BindVertexArray(vaoID);
            }
        }

    private:
        Glid<deleteVertexArray> vaoID;
    };
}