#include <iostream>
#include <stdexcept>

#include <glload/gl_3_3.hpp>
#include <glload/gl_load.hpp>
#include "GLId.h"

//#include "Texture.hpp"
#include "VertexAttributeArray.h"
//#include "VertexBuffer.h"
#include <vector>

#include "WinAPIOpenGLWindow/OpenGLWindow.hpp"

int main() {
    OpenGLWindow win;

    glload::LoadFunctions();

    try {
        gldr::VertexAttributeArray vao;
    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }

    win.setKeydownCb([&win](int key){ printf("lol"); });

    while (win.process());

    /*try {
        gldr::Texture2d tex;
    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }*/
}