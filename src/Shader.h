#pragma once

//#include <glm/glm.hpp>

// standard headers 
#include <map>
#include <ostream>
#include <string>
#include <stdexcept>
#include <boost/range.hpp>
#include <vector>
#include <memory>

#include "glid.hpp"
#include "Util.hpp"

namespace gldr {

enum class ShaderType : GLenum {
    VertexShader = gl::VERTEX_SHADER,
    FragmentShader = gl::FRAGMENT_SHADER,
    GeometryShader = gl::GEOMETRY_SHADER,
    TessControlShader = gl::TESS_CONTROL_SHADER,
    TessEvaluationShader = gl::TESS_EVALUATION_SHADER,
};

static std::string getShaderName(ShaderType type) {
    switch (type) {
    case ShaderType::VertexShader:
        return "Vertex Shader";
    case ShaderType::FragmentShader:
        return "Fragment Shader";
    case ShaderType::GeometryShader:
        return "Geometry Shader";        
    case ShaderType::TessControlShader:
        return "Tesselation Control Shader";
    case ShaderType::TessEvaluationShader:
        return "Tesselation Evaluation Shader";
    default:
        return "Unknown Shader Type";
    }
}

template<ShaderType type>
class Shader
{
protected:
    Glid<Shader<type>> id;
    std::string source;

    std::string _getInfo(unsigned num) const
    {
        GLint blen = 0;    
        GLsizei slen = 0;

        gl::GetShaderiv(num, gl::INFO_LOG_LENGTH , &blen);       

        if (blen > 0)
        {
            char* compiler_log = new char [blen];

            gl::GetInfoLogARB(num, blen, &slen, compiler_log);

            std::string CompilerLogStr (compiler_log);
            delete[] compiler_log;

            return CompilerLogStr;
        }
        return std::string("No error message");
    }

public:
    static GLuint create() {
        GLuint id = gl::CreateShader(static_cast<GLenum>(type));
        if (!id)
            throw std::runtime_error("Problem creating a texture");
        return id;
    }
    static void destroy(GLuint id) {
        gl::DeleteShader(id);
    }

    template<typename Range>
    static Shader<type> load (Range const& range)
    {
        Shader<type> temp;
        temp.source = std::string(boost::begin(range), boost::end(range));
        return std::move(temp);
    }
    template<typename Range>
    Shader(Range const& range) {
        source = std::string(boost::begin(range), boost::end(range));
        compile();
    }
    
    bool isCompiled() const {
        GLint compiled;
        gl::GetShaderiv(id.get(), gl::COMPILE_STATUS, &compiled);
        return compiled == gl::TRUE_;
    }
    explicit operator bool() const {
        return isCompiled();
    }
    std::string getStatus() const {
        if (!isCompiled()) {
            std::string ret = getShaderName(type) +" compilation error : " + _getInfo(id.get());
            return ret;
        }
        return std::string();
    }
    void compile() {
        util::glShaderSource_gldr(id.get(), source);
        gl::CompileShader(id.get());
        if (!isCompiled()) {
            throw std::runtime_error(getStatus().c_str());
        }
    }

    friend class Program;
};

typedef Shader<ShaderType::VertexShader> VertexShader;
typedef Shader<ShaderType::FragmentShader> FragmentShader;

} // namespace gldr
