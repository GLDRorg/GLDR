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

#include "GLId.h"

namespace gldr {

namespace {

void glShaderSource_gldr (GLuint shader, std::string const& shaderSource);
void glShaderSource_gldr (GLuint shader, std::vector<char> const& shaderSource);

}

enum class ShaderType : GLenum {
    VertexShader = gl::VERTEX_SHADER,
    FragmentShader = gl::FRAGMENT_SHADER,
    GeometryShader = gl::GEOMETRY_SHADER
};

static std::string getShaderName(ShaderType type) {
    switch (type) {
    case ShaderType::VertexShader:
        return "Vertex Shader";
    case ShaderType::FragmentShader:
        return "Vertex Shader";
    case ShaderType::GeometryShader:
        return "Vertex Shader";        
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
    }
    
    void compile() {
        glShaderSource_gldr(m_id, m_source);
        glCompileShader(m_id);
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

    friend class Program;
};

typedef Shader<ShaderType::VertexShader> VertexShader;
typedef Shader<ShaderType::FragmentShader> FragmentShader;

class Program
{
    Glid<Program> id;

    std::string _getInfo(unsigned num);
    std::map<std::string, int> m_vertexAttribs;
    std::map<std::string, int> m_uniformCache;

    std::shared_ptr<VertexShader> m_vertexShader;
    std::shared_ptr<FragmentShader> m_fragmentShader;

public:
    static GLuint create() {
        GLuint id = gl::CreateProgram();
        if (!id)
            throw std::runtime_error("Problem creating a texture");
        return id;
    }
    static void destroy(GLuint id) {
        gl::DeleteProgram(id);
    }

    void Unload ();

    void bind();
    void setUniform (const std::string& name, float a);
    void setTex (const std::string& name, unsigned texUnitNum);
    /*void SetUniform (std::string const& name, glm::vec2 const& vec);
    void SetUniform (std::string const& name, glm::vec3 const& vec);
    void SetUniform (std::string const& name, glm::vec4 const& color);
    void SetUniform (std::string const& name, glm::mat4 const& mat);*/

    int GetUniformLocation (const std::string& name);
    int GetAttribLocation (const std::string& name);
    void bindAttribLocation (const std::string& name, int location);

    void attachShader(std::shared_ptr<VertexShader> shader) {
        if (!shader->getStatus().empty())
            throw std::runtime_error("Trying to attach a shader that's not compiled");

        m_vertexShader = std::move(shader);
        gl::AttachShader(id.get(), m_vertexShader->id.get());
    }
    void attachShader(std::shared_ptr<FragmentShader> shader) {
        if (!shader->getStatus().empty())
            throw std::runtime_error("Trying to attach a shader that's not compiled");

        m_fragmentShader = std::move(shader);
        gl::AttachShader(id.get(), m_fragmentShader->id.get());
    }

    static void DisableAll();

    void DebugDump (std::ostream&);

    bool Validate ();
    std::string Link();
    explicit operator bool() {
        return Validate();
    }
    
    Program() { }

    Program(Program&& p) {
        m_vertexAttribs = std::move(p.m_vertexAttribs);
        m_fragmentShader = std::move(p.m_fragmentShader);
        m_vertexShader = std::move(p.m_vertexShader);
        m_uniformCache = std::move(p.m_uniformCache);
        id = std::move(p.id);
    }
};

namespace {

void glShaderSource_gldr (GLuint shader, std::string const& shaderSource)
{
    if (shaderSource.empty())
        throw std::runtime_error("Empty shader passed to `glShaderSource`");
    const GLchar* ptr = shaderSource.c_str();
    const GLint size = shaderSource.size();
    gl::ShaderSource(shader, 1, &ptr, &size);
}

void glShaderSource_gldr (GLuint shader, std::vector<char> const& shaderSource)
{
    if (shaderSource.empty())
        throw std::runtime_error("Empty shader passed to `glShaderSource`");
    const GLchar* ptr = &shaderSource[0];
    const GLint size = shaderSource.size();
    gl::ShaderSource(shader, 1, &ptr, &size);
}

} // anonymous namespace


    using namespace std;

    /*using glm::vec3;
    using glm::vec2;
    using glm::vec4;
    using glm::mat4;
    */

    namespace {
        template<class T> T& as_lvalue(T && v){ return v; }
    }

    std::string Program::_getInfo(unsigned num)
    {
        GLint blen = 0;
        GLsizei slen = 0;

        gl::GetShaderiv(num, gl::INFO_LOG_LENGTH, &blen);

        if (blen > 0)
        {
            char* compiler_log = new char[blen];

            gl::GetInfoLogARB(num, blen, &slen, compiler_log);

            std::string CompilerLogStr(compiler_log);
            delete [] compiler_log;

            return CompilerLogStr;
        }
        return std::string("No error message");
    }

    std::string Program::Link()
    {
        // After recompilations, attributes need to be rebound
        for (auto const& attrib : m_vertexAttribs)
        {
            gl::BindAttribLocation(id.get(), attrib.second, attrib.first.c_str());
        }

        // bind the output
        //gl:BindFragDataLocation(m_id, 0, "out_FragColor");

        unsigned int Shaders[4];
        GLsizei Count;

        // We don't want to bind the shaders twice, if it's just recompilation
        gl::GetAttachedShaders(id.get(), 4, &Count, Shaders);

        /*    if (m_vertexShader)
        glAttachShader(m_id, m_vertexShader->getId());*/
        gl::LinkProgram(id.get());

        GLint linked;
        gl::GetProgramiv(id.get(), gl::LINK_STATUS, &linked);
        if (linked != gl::TRUE_)
            return "Program link error : " + _getInfo(id.get());

        // Success
        return string();
    }

    bool Program::Validate()
    {
        int isValid;
        gl::ValidateProgram(id.get());
        gl::GetProgramiv(id.get(), gl::VALIDATE_STATUS, &isValid);
        return (isValid == gl::TRUE_);
    }


    int Program::GetAttribLocation(const std::string& name)
    {
        return gl::GetAttribLocation(id.get(), name.c_str());
    }

    void Program::bind() {
        gl::UseProgram(id.get());
    }

    /*void Program::SetUniform(const string& name, float a)
    {
        GLint Loc = GetUniformLocation(name);
        if (Loc != -1)
        {
            bind();
            glUniform1fv(Loc, 1, &a);
        }
    }

    void Program::SetUniform(std::string const& name, vec2 const& vec)
    {
        GLint Loc = GetUniformLocation(name);
        if (Loc != -1)
        {
            bind();
            glUniform2fv(Loc, 1, glm::value_ptr(vec));
        }
    }

    void Program::SetUniform(std::string const& name, vec3 const& vec)
    {
        GLint Loc = GetUniformLocation(name);
        if (Loc != -1)
        {
            bind();
            glUniform3fv(Loc, 1, glm::value_ptr(vec));
        }
    }

    void Program::SetUniform(std::string const& name, mat4 const& mat)
    {
        GLint Loc = GetUniformLocation(name);
        if (Loc != -1)
        {
            bind();
            glUniformMatrix4fv(Loc, 1, GL_FALSE, glm::value_ptr(mat));
        }
    }

    void Program::SetUniform(std::string const& name, vec4 const& color)
    {
        GLint Loc = GetUniformLocation(name);
        if (Loc != -1)
        {
            bind();
            glUniform4fv(Loc, 1, glm::value_ptr(color));
        }
    }
    */

    /*int Program::GetUniformLocation(const string& name)
    {
        auto Iter = m_uniformCache.find(name);
        if (Iter != m_uniformCache.end())
            return Iter->second;
        else
        {
            _generateId();
            int Location = glGetUniformLocation(m_id, name.c_str());
            m_uniformCache.insert(std::make_pair(name, Location));
            return Location;
        }
    }

    void Program::SetTex(const std::string& name, unsigned texUnitNum)
    {
        int my_sampler_uniform_location = GetUniformLocation(name);

        bind();
        glUniform1i(my_sampler_uniform_location, texUnitNum);
    }

    void Program::DisableAll()
    {
        glUseProgram(0);
    }

    void Program::bindAttribLocation(std::string const& name, int location)
    {
        m_vertexAttribs[name] = location;
    }

    void Program::DebugDump(std::ostream& Out)
    {
        int count;
        const int MAX_NAME_SIZE = 30;
        char Name[MAX_NAME_SIZE];
        int Len, Size;
        GLenum Type;

        glGetObjectParameterivARB(m_id, GL_OBJECT_ACTIVE_ATTRIBUTES_ARB, &count);
        Out << "Attributes Count = " << count << endl;
        for (int i = 0; i < count; ++i)
        {
            glGetActiveAttrib(m_id, i, MAX_NAME_SIZE, &Len, &Size, &Type, Name);
            Out << i << ". " << Name << " " << Type << "(" << Size << ")\n";
        }

        Out << endl;

        glGetObjectParameterivARB(m_id, GL_OBJECT_ACTIVE_UNIFORMS_ARB, &count);
        Out << "Uniforms Count = " << count << endl;
        for (int i = 0; i < count; ++i)
        {
            glGetActiveUniform(m_id, i, MAX_NAME_SIZE, &Len, &Size, &Type, Name);
            Out << i << ". " << Name << " " << Type << "(" << Size << ")\n";
        }
    }
    */

} // namespace engine
