#include <iostream>
#include <stdexcept>
#include "GLId.h"
#include "Texture.hpp"
#include "VertexAttributeArray.h"
#include "VertexBuffer.h"
#include <vector>

int main() {
    try {
		gldr::Texture2d tex;
    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }
}