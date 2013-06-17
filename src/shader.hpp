#pragma once
namespace gldr {
    struct Shader{
        Shader(const std::string& source, GLenum shaderType):
            shader(gl::CreateShader(shaderType))
        {
            const GLchar* source_glcstr = static_cast<const GLchar*>(source.c_str());
            gl::ShaderSource(shader, 1, &source_glcstr, NULL);
            gl::CompileShader(shader);
        }

        ~Shader(){
            gl::DeleteShader(shader);            
        }

        GLint didCompile(){
            GLint test;
            gl::GetShaderiv(shader, gl::GL_COMPILE_STATUS, &test);
            return test;
        }

        std::string getLog(){
            int logSize = 512;
            std::vector<GLchar> log(logSize);
            gl::GetShaderInfoLog(shader, logSize, NULL, log.data());
            return std::string(static_cast<char*>(log.data()));
        }
    private:
        GLuint shader;
        friend class Program;
    };

    struct Program{
        Program(const std::string& vertexShaderCode, const std::string& fragShaderCode):
            program(gl::CreateProgram())
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
            gl::AttachShader(program, vertShader.shader);
            gl::AttachShader(program, fragShader.shader);
            gl::LinkProgram(program);
        }

        ~Program(){
            gl::DeleteProgram(program);
        }

        GLint getAttribLocation(const std::string& attrib){
            return gl::GetAttribLocation(program, attrib.c_str());
        }

        void use(){
            gl::UseProgram(program);
        }
    private:
        GLuint program;
    };
}
