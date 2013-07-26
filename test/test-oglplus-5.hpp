#pragma once
#include "test.hpp"

class TestOGLPlus5 : public TestBase {
    ProgramGLM prog;
    gldr::VertexAttributeArray vao;
    gldr::VertexBuffer<> verts, coords;

public:
    TestOGLPlus5() {
        gldr::VertexShader vs (std::string(R"( 
            #version 430 core
            layout(location = 0) in vec2 Position;
            layout(location = 1) in vec2 Coord;

            out vec2 vertCoord;

            void main(void)
            {
                vertCoord = Coord;
                gl_Position = vec4(Position, 0.0, 1.0);
            }
        )"));

        gldr::FragmentShader fs (std::string(R"(
            #version 330
            in vec2 vertCoord;
            out vec4 fragColor;

            const int nclr = 5;
            uniform vec4 clrs[5];

            void main(void)
            {
                vec2 z = vec2(0.0, 0.0);
                vec2 c = vertCoord;
                int i = 0, max = 128;
                while ((i != max) && (distance(z, c) < 2.0))
                {
                    vec2 zn = vec2(
                        z.x * z.x - z.y * z.y + c.x,
                        2.0 * z.x * z.y + c.y
                        );
                    z = zn;
                    ++i;
                }
                float a = sqrt(float(i) / float(max));
                for (i = 0; i != (nclr - 1); ++i)
                {
                    if (a >= clrs[i].a && a < clrs[i + 1].a)
                    {
                        float m = (a - clrs[i].a) / (clrs[i + 1].a - clrs[i].a);
                        fragColor = vec4(
                            mix(clrs[i].rgb, clrs[i + 1].rgb, m),
                            1.0
                            );
                        break;
                    }
                }
            }
        )"));
        // compile it
        // attach the shaders to the program
        prog.attachShader(vs);
        prog.attachShader(fs);
        // link and use it
        prog.link();

        GLfloat rectangle_verts[8] = {
            -1.0f, -1.0f,
            -1.0f, 1.0f,
            1.0f, -1.0f,
            1.0f, 1.0f
        };
        
        // upload the data
        verts.data(rectangle_verts);
        // setup the vertex attribs array for the vertices
        // (prog|"Position") is equivalent to VertexAttribArray(prog, "Position")
        //(prog | "Position").Setup<Vec2f>().Enable();
        vao.enableAttributeArray(0);
        vao.directVertexAttribOffset(verts, 0, 2, gldr::VertexAttributeType::Float, false, 0, 0);

        GLfloat rectangle_coords[8] = {
            -1.5f, -0.5f,
            -1.5f, 1.0f,
            0.5f, -0.5f,
            0.5f, 1.0f
        };

        // upload the data
        coords.data(rectangle_coords);
        // setup the vertex attribs array for the vertices
        // (prog|"Coord") is equivalent to VertexAttribArray(prog, "Coord")
        //(prog | "Coord").Setup<Vec2f>().Enable();
        vao.enableAttributeArray(1);
        vao.directVertexAttribOffset(coords, 1, 2, gldr::VertexAttributeType::Float, false, 0, 0);
        // color map used in the fragment shader to colorize the fractal
        const std::size_t nclr = 5;
        std::vector<glm::vec4> colormap {
            glm::vec4 { 0.4f, 0.2f, 1.0f, 0.00f },
            glm::vec4 { 1.0f, 0.2f, 0.2f, 0.30f },
            glm::vec4 { 1.0f, 1.0f, 1.0f, 0.95f },
            glm::vec4 { 1.0f, 1.0f, 1.0f, 0.98f },
            glm::vec4 { 0.1f, 0.1f, 0.1f, 1.00f },
        };
        prog.setUniform("clrs", colormap);
        //
        gl::ClearDepth(1.0);
    }

    void render() {
        vao.bind();
        prog.bind();
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
        // PrimitiveType::TriangleStrip,
        gl::DrawArrays(gl::TRIANGLE_STRIP, 0, 4);
    }
};
