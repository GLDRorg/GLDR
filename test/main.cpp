#include <iostream>
#include <stdexcept>
#include <vector>
#include <boost/exception/all.hpp>

#include <glload/gl_3_3.hpp>
#include <glload/gl_load.hpp>
#include "GLId.h"

//#include "Texture.hpp"
#include "VertexAttributeArray.h"
#include "VertexBuffer.h"
#include "Shader.h"

#include "WinAPIOpenGLWindow/OpenGLWindow.hpp"

typedef boost::error_info<struct tag_my_info, std::string> str_info;
struct ContextException : public virtual std::exception, virtual boost::exception {};


int main() {
    try {
        oglw::Window win;

        glload::LoadFunctions();

        if (!gl::exts::var_ARB_debug_output)
            throw ContextException() << str_info("ARB_debug_output not available");
        if (!gl::exts::var_EXT_direct_state_access)
            throw ContextException() << str_info("EXT_direct_state_access output not available");

        //BOOST_THROW_EXCEPTION(ContextException() << str_info("test"));
        
        win.mousedownCallback = [](oglw::MouseInfo m){
        };

        win.keydownCallback = [&win](oglw::KeyInfo k){
            switch (k.key) {
            case VK_ESCAPE:
                win.close();
                break;
            default:
                printf("lol");
            }
        };

#define NL "\n"
        gldr::Program program;
        program.attachShader(std::make_shared<gldr::VertexShader>(std::string(
            "#version 430 core"
            NL"layout(location = 0) in vec2 position;"
            NL"void main() {"
            NL"    gl_Position = vec4(position, 0.0, 1.0);"
            NL"}"
            NL
            )));
        program.attachShader(std::make_shared<gldr::FragmentShader>(std::string(
            "#version 430 core"
            NL"layout(location = 0) out vec4 fragColor;"
            NL"void main() {"
            NL"    fragColor = vec4(0.8, 0.2, 0.3, 1.0);"
            NL"}"
            NL
        )));
        program.link();
        program.bind();
            //gldr::Texture2d tex;

            // CASE 1
            //{
        std::vector<float> data {
                -0.75f, -0.75f,
                -0.75f,  0.75f,
                 0.75f,  0.75f
        };

        gldr::VertexBuffer<gldr::VertexBufferType::DATA_BUFFER> vbo;
        vbo.data(data);

        gldr::VertexAttributeArray vao;

        vao.enableAttributeArray(0);
        //vao.vertexAttribOffset(0, 2, gldr::VertexAttributeType::Float, false, 0, 0);
        vao.directVertexAttribOffset(1, 0, 2, gldr::VertexAttributeType::Float, false, 0, 0);

        // TODO: indirect drawing with vao's and type and whatnot
        vao.bind();
        
        gl::Disable(gl::CULL_FACE);
        gl::Disable(gl::DEPTH_TEST);
        gl::ClearColor(0.f, 0.4f, 0.2f, 1.f);

        win.displayFunc = [] {
            gl::Clear(gl::COLOR_BUFFER_BIT);
            gl::DrawArrays(gl::TRIANGLES, 0, 3);
        };


        while ( win.display(), win.process() );
    }
    catch (const boost::exception& e) {
        std::cerr << boost::current_exception_diagnostic_information();
    }
    catch (const std::exception& e) {
        std::cerr << boost::current_exception_diagnostic_information();
    }
}