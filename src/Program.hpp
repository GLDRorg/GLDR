#pragma once

#include <map>
#include <memory>
#include <sstream>

#include "Bindable.hpp"
#include "Shader.h"

#define GLDR_HAS_DSA

namespace gldr {

class Program 
    : public Bindable<Program>
  //  , public ProgramExtensionGLM
{
    std::map<std::string, GLuint> m_vertexAttribs;
    std::map<std::string, int> m_uniformCache;
    std::map<std::string, GLuint> fragDataLocations;

    std::string _getInfo(unsigned num) {
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

public:
    static GLuint create() {
        GLuint id = gl::CreateProgram();
        if (!id)
            throw std::runtime_error("Problem creating a shader");
        return id;
    }
    static void destroy(GLuint id) {
        gl::DeleteProgram(id);
    }
    static void bindObject(GLuint id) {
        gl::UseProgram(id);
    }
    static GLuint getCurrentlyBound() {
        return util::getState(gl::CURRENT_PROGRAM);
    }

    void setUniform(const std::string& name, float a) {
        GLint location = getUniformLocation(name);
        if (location != -1) {
            gl::ProgramUniform1f(id.get(), location, a);
        }
    }

    void setTex(const std::string& name, unsigned texUnitNum) {
        int sampler_uniform_location = getUniformLocation(name);
        if (sampler_uniform_location != -1) {
            gl::ProgramUniform1i(id.get(), sampler_uniform_location, texUnitNum);
        }
    }

    int getUniformLocation(const std::string& name) {
        auto Iter = m_uniformCache.find(name);
        if (Iter != m_uniformCache.end())
            return Iter->second;
        else {
            int location = gl::GetUniformLocation(id.get(), name.c_str());
            m_uniformCache.insert(std::make_pair(name, location));
            return location;
        }
    }

    int getAttribLocation(const std::string& name) {
        return gl::GetAttribLocation(id.get(), name.c_str());
    }

    void bindAttribLocation(const std::string& name, int location) {
        m_vertexAttribs[name] = location;
    }
    void bindFragDataLocation(const std::string& name, GLuint location) {
        fragDataLocations[name] = location;
    }

    template<ShaderType shaderType>
    void attachShader(Shader<shaderType> const& shader) {
        gl::AttachShader(id.get(), shader.id.get());
    }

    std::string debugDump() {
        int count;
        const int MAX_NAME_SIZE = 30;
        char Name[MAX_NAME_SIZE];
        int Len, Size;
        GLenum Type;

        std::stringstream out;

        gl::GetObjectParameterivARB(id.get(), gl::OBJECT_ACTIVE_ATTRIBUTES_ARB, &count);
        out << "Attributes Count = " << count << '\n';
        for (int i = 0; i < count; ++i) {
            gl::GetActiveAttrib(id.get(), i, MAX_NAME_SIZE, &Len, &Size, &Type, Name);
            out << i << ". " << Name << " " << Type << "(" << Size << ")\n";
        }
        out << '\n';

        gl::GetObjectParameterivARB(id.get(), gl::OBJECT_ACTIVE_UNIFORMS_ARB, &count);
        out << "Uniforms Count = " << count << '\n';
        for (int i = 0; i < count; ++i) {
            gl::GetActiveUniform(id.get(), i, MAX_NAME_SIZE, &Len, &Size, &Type, Name);
            out << i << ". " << Name << " " << Type << "(" << Size << ")\n";
        }
        out << '\n';
    }

    bool Program::isLinked() {
        GLint linked;
        gl::GetProgramiv(id.get(), gl::LINK_STATUS, &linked);
        return linked == gl::TRUE_;
    }
    bool Program::isValidWithinCurrentState() {
        int isValid;
        gl::ValidateProgram(id.get());
        gl::GetProgramiv(id.get(), gl::VALIDATE_STATUS, &isValid);
        return isValid == gl::TRUE_;
    }
    void link() {
        // After recompilations, attributes need to be rebound
        for (auto const& attrib : m_vertexAttribs) {
            gl::BindAttribLocation(id.get(), attrib.second, attrib.first.c_str());
        }

        for (auto const& frag_loc : fragDataLocations) {
            gl::BindFragDataLocation(id.get(), frag_loc.second, frag_loc.first.c_str());
        }

        /*GLuint Shaders[500];
        GLsizei Count;
        gl::GetAttachedShaders(id.get(), 500, &Count, Shaders);*/

        gl::LinkProgram(id.get());

        if (!isLinked())
            throw std::runtime_error("Program link error : " + _getInfo(id.get()));
    }
    /*explicit operator bool() {
    // TODO: Linked or valid?
    return isValidWithCurrentState();
    }*/

    Program() { }

    Program(Program && p) {
        m_vertexAttribs = std::move(p.m_vertexAttribs);
        m_uniformCache = std::move(p.m_uniformCache);
        id = std::move(p.id);
    }
};

using namespace std;

/*using glm::vec3;
using glm::vec2;
using glm::vec4;
using glm::mat4;
*/

namespace {
    template<class T> T& as_lvalue(T && v){ return v; }
}

/*class ProgramExtensionGLM {

    void SetUniform(std::string const& name, glm::vec2 const& vec);
    void SetUniform(std::string const& name, glm::vec3 const& vec);
    void SetUniform(std::string const& name, glm::vec4 const& color);
    void SetUniform(std::string const& name, glm::mat4 const& mat);

};*/

/*
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


}

