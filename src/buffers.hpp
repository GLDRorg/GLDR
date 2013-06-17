namespace gldr{
    struct VertexBuffer{
        VertexBuffer(){
            gl::GenBuffers(1, &vbo);
        }

        ~VertexBuffer(){
            gl::DeleteBuffers(1, &vbo);
        }

        void bufferData(std::vector<GLfloat> data){
            bind();
            gl::BufferData(gl::GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), data.data(), gl::GL_STATIC_DRAW);
        }

        void bind(){            
            gl::BindBuffer(gl::GL_ARRAY_BUFFER, vbo);
        }

    private:
        GLuint vbo;
    };

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