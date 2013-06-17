#pragma once
namespace gldr{
    struct VertexBuffer{
        enum class BufferType : GLuint{
            DATA  = gl::GL_ARRAY_BUFFER,
            INDEX = gl::GL_ELEMENT_ARRAY_BUFFER
        };

        enum class Usage : GLuint{
            STATIC_DRAW  = gl::GL_STATIC_DRAW,
            DYNAMIC_DRAW = gl::GL_DYNAMIC_DRAW
            /* STATIC_READ, STATIC_WRITE, DYNAMIC_READ, DYNAMIC_WRITE*/
        };

        VertexBuffer(BufferType bufferType = BufferType::DATA, Usage usage = Usage::STATIC_DRAW):
            bufferType(bufferType), usage(usage)
        {
            gl::GenBuffers(1, &vbo);
        }

        ~VertexBuffer(){
            gl::DeleteBuffers(1, &vbo);
        }

        void bufferData(std::vector<GLfloat> data){
            bind();
            gl::BufferData(static_cast<GLuint>(bufferType), sizeof(GLfloat) * data.size(), data.data(), static_cast<GLuint>(usage));
        }

        void bind(){            
            gl::BindBuffer(static_cast<GLuint>(bufferType), vbo);
        }

    private:
        GLuint vbo;
        BufferType bufferType;
        Usage usage;
    };
}