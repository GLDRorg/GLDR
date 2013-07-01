#pragma once
#include "shader.hpp"
#include <iostream>
namespace gldr {
    struct Program{
        Program(const std::string& vertexShaderCode, const std::string& fragShaderCode){
            VertexShader vertShader(vertexShaderCode);
            FragmentShader fragShader(fragShaderCode);
            if(!vertShader.didCompile()){
                std::cerr << "Vertex shader failed to compile!" << std::endl;
                std::cerr << vertShader.getLog() << std::endl;
            }
            if(!fragShader.didCompile()){
                std::cerr << "Fragment shader failed to compile!" << std::endl;
                std::cerr << fragShader.getLog() << std::endl;
            }
            gl::AttachShader(programID, vertShader.shaderID);
            gl::AttachShader(programID, fragShader.shaderID);
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