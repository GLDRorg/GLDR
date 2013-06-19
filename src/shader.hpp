#pragma once
#include "glid.hpp"
namespace gldr {
    struct Shader{
        Shader(const std::string& source, GLenum shaderType):
            shaderID(gl::CreateShader(shaderType), gl::DeleteShader)
        {
            const GLchar* source_glcstr = static_cast<const GLchar*>(source.c_str());
            gl::ShaderSource(shaderID.get(), 1, &source_glcstr, NULL);
            gl::CompileShader(shaderID.get());
        }

        GLint didCompile() const {
            GLint test = gl::GL_FALSE;
            // if shaderID = 0 test will remane GL_FALSE and error GL_INVALID_VALUE will be set
            gl::GetShaderiv(shaderID.get(), gl::GL_COMPILE_STATUS, &test);
            return test;
        }

        std::string getLog() const {
            int logSize = 512;
            std::vector<GLchar> log(logSize);
            // if shaderID = 0 log will remane empty and error GL_INVALID_VALUE will be set
            gl::GetShaderInfoLog(shaderID.get(), logSize, NULL, log.data());
            return std::string(static_cast<char*>(log.data()));
        }

    private:
        Glid shaderID;
        friend class Program;
    };

    struct Program{
        Program(const std::string& vertexShaderCode, const std::string& fragShaderCode):
            program(gl::CreateProgram(), gl::DeleteProgram)
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
            gl::AttachShader(program.get(), vertShader.shaderID.get());
            gl::AttachShader(program.get(), fragShader.shaderID.get());
            gl::LinkProgram(program.get());
        }

        GLint getAttribLocation(const std::string& attrib) const{
            if(program.get()){
                return gl::GetAttribLocation(program.get(), attrib.c_str());
            }
            return -1;
        }

        void use() const{
            if(program.get()){
                gl::UseProgram(program.get());
            }
        }
    private:
        Glid program;
    };
}
