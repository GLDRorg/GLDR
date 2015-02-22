#pragma once
#include "glid.hpp"
#include <string>
#include <vector>
namespace gldr {
namespace shaderOptions{
    enum class Type : GLuint{
        Vertex = gl::VERTEX_SHADER,
        Fragment = gl::FRAGMENT_SHADER,
    };
}
template<shaderOptions::Type type>
struct Shader{
    Shader(const std::string& source){
        const GLchar* source_glcstr = static_cast<const GLchar*>(source.c_str());
        gl::ShaderSource(shaderID.get(), 1, &source_glcstr, NULL);
        gl::CompileShader(shaderID.get());
    }

    bool didCompile() const{
        GLint test = GL_FALSE;
        // if shaderID = 0 test will remane FALSE and error INVALID_VALUE will be set
        gl::GetShaderiv(shaderID.get(), gl::COMPILE_STATUS, &test);
        return test == GL_TRUE;
    }

    std::string getLog() const{
        int logSize = 512;
        std::vector<GLchar> log(logSize);
        // if shaderID = 0 log will remane empty and error INVALID_VALUE will be set
        gl::GetShaderInfoLog(shaderID.get(), logSize, NULL, log.data());
        return std::string(static_cast<char*>(log.data()));
    }

    static GLuint create(){
        return gl::CreateShader(static_cast<GLuint>(type)); 
    }        

    static void destroy(GLuint& id){
        gl::DeleteShader(id);
    }
    
private:
    Glid<Shader<type>> shaderID;
    friend class Program;
};

typedef Shader<shaderOptions::Type::Vertex> VertexShader;
typedef Shader<shaderOptions::Type::Fragment> FragmentShader;
}
