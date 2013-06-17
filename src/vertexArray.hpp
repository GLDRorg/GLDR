#pragma once
namespace gldr{
    struct VertexArray{
        VertexArray(){
            gl::GenVertexArrays(1, &vao);
        }

        ~VertexArray(){
            gl::DeleteVertexArrays(1, &vao);
        }

        void bind(){
            gl::BindVertexArray(vao);
        }

    private:
        GLuint vao;
    };
}