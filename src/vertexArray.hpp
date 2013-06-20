#pragma once
#include "glid.hpp"
namespace gldr{
    struct VertexArray{
        VertexArray():
            vaoID(&gl::DeleteVertexArrays){
            gl::GenVertexArrays(1, vaoID.ptr());
        }

        void bind() const {
            if(vaoID){
                gl::BindVertexArray(vaoID);
            }
        }

    private:
        Glid<decltype(&gl::DeleteVertexArrays)> vaoID;
    };
}