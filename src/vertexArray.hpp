#pragma once
#include "glid.hpp"
namespace gldr{
struct VertexArray{
    void bind() const{
        if(static_cast<GLuint>(vaoID)){
            gl::BindVertexArray(static_cast<GLuint>(vaoID));
        }
    }

    static GLuint create(){
        GLuint id;
        gl::GenVertexArrays(1, &id);
        return id; 
    }        

    static void destroy(GLuint& id){
        gl::DeleteVertexArrays(1, &id);
    }
private:
    Glid<VertexArray> vaoID;
};
}