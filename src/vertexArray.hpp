#pragma once
#include "glid.hpp"
namespace gldr{
    struct VertexArray{
        VertexArray(){}

        void bind() const{
            if(vaoID){
                gl::BindVertexArray(vaoID);
            }
        }

        static GLuint creater(){
            GLuint id;
            gl::GenVertexArrays(1, &id);
            return id; 
        }        

        static void deleter(GLuint& id){
            gl::DeleteVertexArrays(1, &id);
        }
    private:
        Glid<VertexArray> vaoID;
    };
}