#pragma once
#include "test.hpp"

#include "oglplus/curve.hpp"
#include <glm/gtc/matrix_transform.hpp>

class TestOGLPlus15Graph : public TestBase {
    gldr::Framebuffer fbo;

    // Program
    ProgramGLM prog;

    // A vertex array object for the rendered cube
    gldr::VertexAttributeArray vao;

    // VBOs for the graph's vertices and edge element indices
    gldr::VertexBuffer<> verts;
    gldr::VertexBuffer<gldr::VertexBufferType::INDEX_BUFFER> edges;

    // Uniform variables
    glm::mat4 projection_matrix;
    glm::mat4 camera_matrix;

    // The number of nodes in the graph
    const GLuint node_count;
    // The number of edges in the graph
    GLuint edge_count;

    static double nrand(void) {
        return 2.0*(double(std::rand()) / RAND_MAX - 0.5);
    }

    oglplus::CubicBezierLoop<glm::vec3, float> cam_path;
    oglplus::CubicBezierLoop<glm::vec3, float> tgt_path;
    static std::vector<glm::vec3> make_cam_path_cps(void)
    {
        const glm::vec3 _points[6] = {
            glm::vec3(-40.0f, -50.0f, -50.0f),
            glm::vec3(40.0f, 0.0f, -60.0f),
            glm::vec3(60.0f, 30.0f, 50.0f),
            glm::vec3(-20.0f, 50.0f, 55.0f),
            glm::vec3(-30.0f, 30.0f, 0.0f),
            glm::vec3(-60.0f, 4.0f, -30.0f)
        };
        return std::vector<glm::vec3>(_points, _points + 6);
    }

    static std::vector<glm::vec3> make_tgt_path_cps(void)
    {
        const glm::vec3 _points[6] = {
            glm::vec3(-10.0f, 0.0f, -10.0f),
            glm::vec3(10.0f, 10.0f, -10.0f),
            glm::vec3(10.0f, 0.0f, 10.0f),
            glm::vec3(-10.0f, -5.0f, 15.0f),
            glm::vec3(-10.0f, -3.0f, 0.0f),
            glm::vec3(-10.0f, 0.0f, -10.0f)
        };
        return std::vector<glm::vec3>(_points, _points + 6);
    }

public:
    TestOGLPlus15Graph()
        : node_count(512)
        , edge_count(0)
        , cam_path(make_cam_path_cps())
        , tgt_path(make_tgt_path_cps())
    {
        std::srand(std::time(0));

        std::srand(std::time(0));

        // Vertex shader
        gldr::VertexShader vs(
            "#version 430\n"
            "uniform mat4 ProjectionMatrix, CameraMatrix;"
            "layout(location = 0) in vec4 Position;"
            "void main(void)"
            "{"
            "	gl_Position = "
            "		ProjectionMatrix *"
            "		CameraMatrix *"
            "		Position;"
            "	gl_PointSize = 4.0 * gl_Position.w / gl_Position.z;"
            "}"
            );

        // Fragment shader
        gldr::FragmentShader fs(
            "#version 430\n"
            "out vec4 fragColor;"
            "void main(void)"
            "{"
            "	fragColor = vec4(0.0, 0.0, 0.0, 1.0);"
            "}"
            );

        // attach the shaders to the program
        prog.attachShader(vs);
        prog.attachShader(fs);
        // link and use it
        prog.link();


        std::vector<GLfloat> positions(node_count * 3);
        // bind the VBO for the cube vertices
        {
            GLuint k = 0;
            for (GLuint p = 0; p != node_count; ++p)
            {
                positions[k++] = nrand() *120.0;
                positions[k++] = nrand() *  5.0;
                positions[k++] = nrand() *120.0;
            }
            assert(k == positions.size());

            // upload the data
            verts.data(positions);
            // setup the vertex attribs array for the vertices
            vao.enableAttributeArray(0);
            vao.directVertexAttribOffset(verts, 0, 3, gldr::VertexAttributeType::Float, false, 0, 0);
        }

        // bind the VBO for cube edge indices
        {
            std::vector<GLuint> edge_data;
            edge_data.reserve(node_count * 6);
            for (GLuint i = 0; i != node_count; ++i)
            {
                glm::vec3 pi(
                    positions[i * 3 + 0],
                    positions[i * 3 + 1],
                    positions[i * 3 + 2]
                    );
                float min_dist = 1000.0f;
                GLuint m = i;
                for (GLuint j = i + 1; j != node_count; ++j)
                {
                    glm::vec3 pj(
                        positions[j * 3 + 0],
                        positions[j * 3 + 1],
                        positions[j * 3 + 2]
                        );
                    float dist = glm::distance(pi, pj);
                    if (min_dist > 1.0 && min_dist > dist)
                    {
                        min_dist = dist;
                        m = j;
                    }
                }
                min_dist *= 2.0f;
                GLuint done = 0;
                for (GLuint j = i + 1; j != node_count; ++j)
                {
                    glm::vec3 pj(
                        positions[j * 3 + 0],
                        positions[j * 3 + 1],
                        positions[j * 3 + 2]
                        );
                    float dist = glm::distance(pi, pj);
                    if (min_dist > dist)
                    {
                        float x = dist / min_dist;
                        if (std::pow(nrand(), 2.0) >= x)
                        {
                            edge_data.push_back(i);
                            edge_data.push_back(j);
                            ++done;
                        }
                    }
                }
                if (done == 0)
                {
                    if (i != m)
                    {
                        edge_data.push_back(i);
                        edge_data.push_back(m);
                    }
                }
            }
            edges.data(edge_data);
            assert(edge_data.size() % 2 == 0);
            edge_count = edge_data.size();

            vao.bind();
            edges.bind();

            positions.clear();
        }

        //
        gl::ClearColor(0.9f, 0.9f, 0.8f, 0.0f);
        gl::ClearDepth(1.0f);
        gl::Enable(gl::DEPTH_TEST);
        gl::Enable(gl::LINE_SMOOTH);
        gl::Enable(gl::PROGRAM_POINT_SIZE);
        gl::Enable(gl::BLEND);;
        gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);

        projection_matrix = glm::perspective(70.f, 8.f / 6.f, 1.f, 200.f);
        prog.setUniform("ProjectionMatrix", projection_matrix);
    }

    void render(double time) override {
        vao.bind();
        prog.bind();

        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
        // set the matrix for camera orbiting the origin
        camera_matrix = glm::lookAt(cam_path.Position(time / 9.0f),
                                    cam_path.Position(time / 7.0f),
                                    glm::vec3(0.f, 1.f, 0.f)
                                    );
        prog.setUniform("CameraMatrix", camera_matrix);
        

        // draw the points
        gl::DrawArrays(gl::POINTS, 0, node_count * 3);
        // draw the edges
        gl::DrawElements(
            gl::LINES,
            edge_count,
            gl::UNSIGNED_INT,
            0);

    }
};