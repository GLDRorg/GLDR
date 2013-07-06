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
        gl::AttachShader(static_cast<GLuint>(programID), static_cast<GLuint>(shader.shaderID));
    }

    template<shaderOptions::Type type, typename... More>
    void attach(const Shader<type>& shader, const More&... others){
        attach(shader);
        attach(others...);
    }

    void link(){
        gl::LinkProgram(static_cast<GLuint>(programID));
    }

    GLint getAttribLocation(const std::string& attrib) const{
        if(static_cast<GLuint>(programID)){
            return gl::GetAttribLocation(static_cast<GLuint>(programID), attrib.c_str());
        }
        return -1;
    }

    GLint getUniformLocation(const std::string& uniform) const{
        if(static_cast<GLuint>(programID)){
            return gl::GetUniformLocation(static_cast<GLuint>(programID), uniform.c_str());
        }
        return -1;
    }

    void use() const{
        if(static_cast<GLuint>(programID)){
            gl::UseProgram(static_cast<GLuint>(programID));
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