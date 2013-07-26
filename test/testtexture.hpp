#pragma once
#include "test.hpp"
#include "TextureEnums.hpp"

class TestTextures : public TestBase {
    gldr::Program program;
    gldr::Texture2d tex;
    gldr::VertexBuffer<> vbo;
    gldr::VertexAttributeArray vao;

public:
    TestTextures() {
        std::vector<float> data {
            -0.75f, -0.75f,
                -0.75f, 0.75f,
                0.75f, 0.75f
        };
        vbo.data(data);

        vao.enableAttributeArray(0);
        vao.directVertexAttribOffset(vbo, 0, 2, gldr::VertexAttributeType::Float, false, 0, 0);

        gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);

        program.attachShader(gldr::VertexShader(std::string(R"(
            #version 430 core
            layout(location = 0) in vec2 position;
            out vec2 texcoord;
            void main() {
                gl_Position = vec4(position, 0.0, 1.0);
                texcoord = position * 5.0;
            }

        )")));
        program.attachShader(gldr::FragmentShader(std::string(R"(
            #version 430 core
            layout(location = 0) out vec4 fragColor;
            in vec2 texcoord;
            uniform sampler2D tex;
            void main() {
                fragColor = vec4(0.8, 0.2, 0.3, 1.0);
                fragColor = texture(tex,texcoord);
            }

        )")));

        program.bindFragDataLocation("fragColor", 0);
        program.link();
        program.bind();
        program.setTex("tex", 0);

        std::vector<unsigned char> texData {
            0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff,
                0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
                0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
        };
        std::vector<unsigned char> texData2 {
            255, 0, 0,
                0, 255, 0,
                0, 0, 255,
                255, 255, 0
        };

        tex.setMagFiltering(gldr::texture_desc::MagFilteringMode::Nearest);
        tex.setMinFiltering(gldr::texture_desc::MinFilteringMode::Nearest);
        tex.bindToUnit(0);

        gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
        tex.imageData(3, 3,
            gldr::texture_desc::Format::RGB,
            gldr::texture_desc::InternalFormat::RGB8,
            gldr::texture_desc::DataType::UnsignedByte,
            texData.data()
            );
    }
    void render(double) override {
        vao.bind();
        gl::Clear(gl::COLOR_BUFFER_BIT);
        gl::DrawArrays(gl::TRIANGLES, 0, 3);
    }
};