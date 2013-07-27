#include <iostream>
#include <stdexcept>

//#include <glload/gl_3_3.hpp>
//#include <glload/gl_load.hpp>
//#include "GLId.h"

//#include <boost/exception.hpp>

//#include "Texture.hpp"
//#include "VertexAttributeArray.h"
//#include "VertexBuffer.h"
#include <vector>

#include "OpenGLWindow.hpp"

int main() {
    try {
        oglw::Window win;

        win.keydownCallback = [&win](oglw::KeyInfo const& k){ printf("lol"); };

        //gldr::VertexAttributeArray vao;
        //gldr::Texture2d tex;

        while ( win.display(), win.process() );
    }
    catch (const std::exception& e) {
        std::cerr << e.what();
    }
}
