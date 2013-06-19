#pragma once
#include <map>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <ostream>
#include <string>
#include <exception>
#include <stdexcept>
#include <boost/range.hpp>
#include <vector>
#include <memory>
#include "GLId.h"

namespace gldr {

class CVector3;
class CMatrix4;
class CColor;

namespace {

void glShaderSource_gldr (GLuint shader, std::string const& shaderSource);
void glShaderSource_gldr (GLuint shader, std::vector<char> const& shaderSource);

}

template<typename T>
class Shader
{
protected:
    GLId m_id;
    std::string m_source;

    std::string _getInfo(unsigned num) const
    {
        GLint blen = 0;    
        GLsizei slen = 0;

        glGetShaderiv(num, GL_INFO_LOG_LENGTH , &blen);       

        if (blen > 0)
        {
            char* compiler_log = new char [blen];

            glGetInfoLogARB(num, blen, &slen, compiler_log);

            std::string CompilerLogStr (compiler_log);
            delete[] compiler_log;

            return CompilerLogStr;
        }
        return std::string("No error message");
    }

public:
    GLuint getId () const { return m_id; }

    template<typename Range>
    static T Load (Range const& range)
    {
        T temp;

        temp.m_source = std::string(boost::begin(range), boost::end(range));
        temp.m_id = glCreateShader(T::_getType());

        return std::move(temp);
    }
    template<typename Range>
    Shader(Range const& range) {
        m_source = std::string(boost::begin(range), boost::end(range));
        m_id = glCreateShader(T::_getType());
    }
    
    std::string Status() const
    {
        GLint compiled;

        glGetShaderiv(m_id, GL_COMPILE_STATUS, &compiled);
        if (compiled != GL_TRUE) {
            std::string ret = T::_getName() + " compilation error : " + _getInfo(m_id);
            return ret;
        }

        return std::string();
    }
    std::string Compile ()
    {
        glShaderSource_gldr(m_id, m_source);
        glCompileShader(m_id);
        return Status();
    }
    void Unload ()
    {
        glDeleteShader(m_id);
    }    

    ~Shader () { Unload(); }

protected:
    Shader() { }
};

class VertexShader : public Shader<VertexShader>
{
    static const std::string _getName() { return "Vertex shader"; }
    static const GLint _getType() { return GL_VERTEX_SHADER; }
public:
    VertexShader() : Shader() { }
    template<typename Range>
    VertexShader(Range range) : Shader(range) { }
    VertexShader(VertexShader&& other) {
        m_source = std::move(other.m_source);
        m_id = std::move(other.m_id);
    }
    
    //template<typename T>
    friend class Shader;

    explicit operator bool() { return Status().empty(); }
};

class FragmentShader : public Shader<FragmentShader>
{
    static const std::string _getName() { return "Fragment shader"; }
    static const GLint _getType() { return GL_FRAGMENT_SHADER; }

public:
    FragmentShader() : Shader() { }
    template<typename Range>
    FragmentShader(Range range) : Shader(range) { }
    FragmentShader(FragmentShader&& other) {
        m_source = std::move(other.m_source);
        m_id = std::move(other.m_id);
    }

    //template<typename T>
    friend class Shader;
    //friend class Shader;

    explicit operator bool() { return Status().empty(); }
};

class Program
{
    GLId m_id;

    std::string _getInfo(unsigned num);
    std::map<std::string, int> m_vertexAttribs;
    std::map<std::string, int> m_uniformCache;

    std::shared_ptr<VertexShader> m_vertexShader;
    std::shared_ptr<FragmentShader> m_fragmentShader;
    
    void _generateId () {
        if (!m_id)
            m_id = glCreateProgram();
    }

public:
    void Unload ();

    void Bind();
    void SetUniform (const std::string& name, float a);
    void SetTex (const std::string& name, unsigned texUnitNum);
    void SetUniform (std::string const& name, glm::vec2 const& vec);
    void SetUniform (std::string const& name, glm::vec3 const& vec);
    void SetUniform (std::string const& name, glm::vec4 const& color);
    void SetUniform (std::string const& name, glm::mat4 const& mat);

    int GetUniformLocation (const std::string& name);
    GLuint GetProgramNum () const { return m_id; }
    int GetAttribLocation (const std::string& name);
    void BindAttribLocation (const std::string& name, int location);

    void AttachShader(std::shared_ptr<VertexShader> shader) {
        _generateId();

        if (!shader->Status().empty())
            throw std::runtime_error("Trying to attach a shader that's not compiled");

        m_vertexShader = std::move(shader);
        glAttachShader(m_id, m_vertexShader->getId());
    }
    void AttachShader(std::shared_ptr<FragmentShader> shader) {
        _generateId();

        if (!shader->Status().empty())
            throw std::runtime_error("Trying to attach a shader that's not compiled");

        m_fragmentShader = std::move(shader);
        glAttachShader(m_id, m_fragmentShader->getId());
    }

    static void DisableAll();

    void DebugDump (std::ostream&);

    bool Validate ();
    std::string Link();
    explicit operator bool();

    Program() : m_id(0) {
    }

    Program(Program&& p) {
        m_vertexAttribs = std::move(p.m_vertexAttribs);
        m_fragmentShader = std::move(p.m_fragmentShader);
        m_vertexShader = std::move(p.m_vertexShader);
        m_uniformCache = std::move(p.m_uniformCache);
        m_id = std::move(p.m_id);
    }

    ~Program() {
        glDeleteProgram(m_id);
    }
};

namespace {

void glShaderSource_gldr (GLuint shader, std::string const& shaderSource)
{
    if (shaderSource.empty())
        throw std::runtime_error("Empty shader passed to `glShaderSource`");
    const GLchar* ptr = shaderSource.c_str();
    const GLint size = shaderSource.size();
    glShaderSource(shader, 1, &ptr, &size);
}

void glShaderSource_gldr (GLuint shader, std::vector<char> const& shaderSource)
{
    if (shaderSource.empty())
        throw std::runtime_error("Empty shader passed to `glShaderSource`");
    const GLchar* ptr = &shaderSource[0];
    const GLint size = shaderSource.size();
    glShaderSource(shader, 1, &ptr, &size);
}

} // anonymous namespace

} // namespace gldr
