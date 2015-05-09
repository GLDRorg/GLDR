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

    bool didLink() const{
        GLint status = GL_FALSE;
        // if programID = 0 status will remane FALSE and error INVALID_VALUE will be set
        gl::GetProgramiv(programID.get(), gl::LINK_STATUS, &status);
        return status != GL_FALSE;
    }

    std::string getLog() const{
        int logSize = 0;
        gl::GetShaderiv(programID.get(), gl::INFO_LOG_LENGTH, &logSize);
        std::vector<GLchar> log(logSize + 1);
        // if programID = 0 log will remane empty and error INVALID_VALUE will be set
        gl::GetShaderInfoLog(programID.get(), logSize, NULL, log.data());
        return std::string(static_cast<char*>(log.data()));
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