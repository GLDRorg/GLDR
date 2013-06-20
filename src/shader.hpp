#pragma once
#include "glid.hpp"
namespace gldr {
    struct Shader{
        Shader(const std::string& source, GLenum shaderType):
            shaderID(gl::DeleteShader, gl::CreateShader(shaderType))
        {
            const GLchar* source_glcstr = static_cast<const GLchar*>(source.c_str());
            gl::ShaderSource(shaderID, 1, &source_glcstr, NULL);
            gl::CompileShader(shaderID);
        }

        GLint didCompile() const {
            GLint test = gl::GL_FALSE;
            // if shaderID = 0 test will remane GL_FALSE and error GL_INVALID_VALUE will be set
            gl::GetShaderiv(shaderID, gl::GL_COMPILE_STATUS, &test);
            return test;
        }

        std::string getLog() const {
            int logSize = 512;
            std::vector<GLchar> log(logSize);
            // if shaderID = 0 log will remane empty and error GL_INVALID_VALUE will be set
            gl::GetShaderInfoLog(shaderID, logSize, NULL, log.data());
            return std::string(static_cast<char*>(log.data()));
        }

    private:
        Glid<decltype(&gl::DeleteShader)> shaderID;
        friend class Program;
    };

    struct Program{
        Program(const std::string& vertexShaderCode, const std::string& fragShaderCode):
            programID(&gl::DeleteProgram, gl::CreateProgram())
        {
            Shader vertShader = Shader(vertexShaderCode, gl::GL_VERTEX_SHADER);
            Shader fragShader = Shader(fragShaderCode, gl::GL_FRAGMENT_SHADER);
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

        void use() const{
            if(programID){
                gl::UseProgram(programID);
            }
        }
    private:
        Glid<decltype(&gl::DeleteProgram)> programID;
    };
}
