#include <iostream>
#include <stdexcept>
#include <vector>
#include <boost/exception/all.hpp>

#include "glid.hpp"

#include "WinAPIOpenGLWindow/OpenGLWindow.hpp"

#include "test.hpp"
#include "debug.hpp"

typedef boost::error_info<struct tag_my_info, std::string> str_info;
struct ContextException : public virtual std::exception, virtual boost::exception {};


int main() {
    try {
        oglw::Window win;

        glload::LoadFunctions();

        gl::Enable(gl::DEBUG_OUTPUT);
        // Debug output is already part of the core
        /*if (!gl::exts::var_ARB_debug_output)
            throw ContextException() << str_info("ARB_debug_output not available");*/
        if (!gl::exts::var_EXT_direct_state_access)
            throw ContextException() << str_info("EXT_direct_state_access not available");

        debugMessageControl(DebugMessageSource::DontCare, DebugMessageType::DontCare, DebugMessageSeverity::DontCare, true);
        //BOOST_THROW_EXCEPTION(ContextException() << str_info("test"));
        
        win.mousedownCallback = [](oglw::MouseInfo m){
            if (m.button == oglw::MouseInfo::Button::Left) {
                std::cout << "left";
            }
            else if (m.button == oglw::MouseInfo::Button::Right){
                std::cout << "right";
            }
        };

        win.keydownCallback = [&win](oglw::KeyInfo k){
            switch (k.key) {
            case VK_ESCAPE:
                win.close();
                break;
            default:
                std::cout << "lol";
            }
        };

        std::unique_ptr<TestBase> test (new TestTextures());

        auto errors = getDebugLog();
        for (auto const& error : errors) {
            std::cout << error;
        }

        // both methods work fine; shader that is attached to 
        // a program will be flagged for deletion, but won't be
        // deleted until there's at least one program referencing
        // it
       
        /*gldr::Sampler s;
        s.samplerParameter(gl::TEXTURE_MAG_FILTER, gl::LINEAR);
        s.samplerParameter(gl::TEXTURE_MIN_FILTER, gl::LINEAR);
        //s.bindToUnit(0);*/
        
        gl::Disable(gl::CULL_FACE);
        gl::Disable(gl::DEPTH_TEST);
        gl::ClearColor(0.4f, 0.4f, 0.2f, 1.f);

        win.displayFunc = [&test] {
            test->render();
        };

        while ( win.display(), win.process() );
    }
    catch (const boost::exception& e) {
        std::cerr << boost::current_exception_diagnostic_information();
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << boost::current_exception_diagnostic_information();
        return 1;
    }
}

void textureTest() {
}