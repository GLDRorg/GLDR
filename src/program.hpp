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
        gl::AttachShader(programID.get(), shader.shaderID.get());
    }

    template<shaderOptions::Type type, typename... More>
    void attach(const Shader<type>& shader, const More&... others){
        attach(shader);
        attach(others...);
    }

    void link(){
        gl::LinkProgram(programID.get());
    }

    GLint getAttribLocation(const std::string& attrib) const{
        if(programID.get()){
            return gl::GetAttribLocation(programID.get(), attrib.c_str());
        }
        return -1;
    }

    GLint getUniformLocation(const std::string& uniform) const{
        if(programID.get()){
            return gl::GetUniformLocation(programID.get(), uniform.c_str());
        }
        return -1;
    }

    void use() const{
        if(programID.get()){
            gl::UseProgram(programID.get());
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