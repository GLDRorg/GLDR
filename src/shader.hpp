#pragma once
#include "glid.hpp"
namespace gldr {
    namespace {
        void deleteShader(GLuint& id){
            gl::DeleteShader(id);
        }
    }
    struct Shader{
        Shader(const std::string& source, GLenum shaderType):
            shaderID(gl::CreateShader(shaderType))
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
        Glid<&deleteShader> shaderID;
        friend class Program;
    };
}
