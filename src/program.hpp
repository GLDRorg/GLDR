#pragma once
#include "shader.hpp"
#include <iostream>
namespace gldr {
struct Program{
    template<shaderOptions::Type type>    
    void attach(const Shader<type>& shader){
        if(!shader.didCompile()){
            std::cerr << "Shader is not compiled or fialed to compile\n";
            std::cerr << shader.getLog() << "\n";
        }
        gl::AttachShader(programID, shader.shaderID);
    }

    void link(){
        gl::LinkProgram(programID);
    }

    GLint getAttribLocation(const std::string& attrib) const{
        if(programID){
            return gl::GetAttribLocation(programID, attrib.c_str());
        }
        return -1;
    }

    GLint getUniformLocation(const std::string& uniform) const{
        if(programID){
            return gl::GetUniformLocation(programID, uniform.c_str());
        }
        return -1;
    }

    void use() const{
        if(programID){
            gl::UseProgram(programID);
        }
    }

    static GLuint create(){
        return gl::CreateProgram(); 
    }        

    static void destroy(GLuint& id){
        gl::DeleteProgram(id); 
    }
    
private:
    Glid<Program> programID;
};
}