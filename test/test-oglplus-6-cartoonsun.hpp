#pragma once
#include "test.hpp"
#include <glm/gtc/constants.hpp>

class TestOGLPlus6CartoonSun : public TestBase {
    ProgramGLM prog;

    gldr::VertexAttributeArray vao;
    gldr::VertexBuffer<> verts;

public:
    TestOGLPlus6CartoonSun() {
        gldr::VertexShader vs(std::string(R"(
            #version 330
            in vec2 Position;
            out vec2 vertPos;
            void main(void)
            {
                gl_Position = vec4(Position, 0.0, 1.0);
                vertPos = gl_Position.xy;
            }
        )"));

        gldr::FragmentShader fs(std::string(R"(
            #version 330
            uniform float Time;
            uniform vec2 SunPos;
            uniform vec3 Sun1, Sun2, Sky1, Sky2;
            in vec2 vertPos;
            out vec3 fragColor;
            void main(void)
            {
                vec2 v = vertPos - SunPos;
                float l = length(v);
                float a = atan(v.y, v.x)/3.1415;
                if(l < 0.1)
                    fragColor = Sun1;
                else if(int(18*(Time*0.1 + 1.0 + a)) % 2 == 0)
                    fragColor = mix(Sun1, Sun2, l);
                else
                    fragColor = mix(Sky1, Sky2, l);
            }
        )"));

        prog.attachShader(vs);
        prog.attachShader(fs);
        prog.link();

        prog.setUniform("Sun1", glm::vec3 { 0.95f, 0.85f, 0.60f });
        prog.setUniform("Sun2", glm::vec3 { 0.90f, 0.80f, 0.20f });
        prog.setUniform("Sky1", glm::vec3 { 0.90f, 0.80f, 0.50f });
        prog.setUniform("Sky2", glm::vec3 { 0.80f, 0.60f, 0.40f });

        GLfloat rectangle_verts[8] = {
            -1.0f, -1.0f,
            -1.0f, 1.0f,
            1.0f, -1.0f,
            1.0f, 1.0f
        };
        verts.data(rectangle_verts);
        
        vao.enableAttributeArray(0);
        vao.directVertexAttribOffset(verts, 0, 2, gldr::VertexAttributeType::Float, false, 0, 0);

        gl::ClearDepth(1.0f);
    }
    void render(double time) override {
        prog.bind();
        vao.bind();
        // Fullcircles convert the data from 360 degrees to its inner type
        //auto angle = FullCircles(GLfloat(time * 0.05f));
        float angle = (GLfloat(time) * 0.05f) * 2.f * glm::pi<float>();

        prog.setUniform("Time", static_cast<float>(time));

        prog.setUniform("SunPos", glm::vec2 {
            -std::cos(angle),
            std::sin(angle)
        });

        gl::DrawArrays(gl::TRIANGLE_STRIP, 0, 4);
    }
};
