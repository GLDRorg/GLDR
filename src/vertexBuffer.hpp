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
            gl::GenBuffers(1, &vboID);
        }

        VertexBuffer(VertexBuffer&& other):
            vboID(other.vboID),
            bufferType(other.bufferType),
            usage(other.usage){
            other.vboID = 0;
        }

        ~VertexBuffer(){
            gl::DeleteBuffers(1, &vboID);
        }

        VertexBuffer& operator= (VertexBuffer&& other) {
            vboID = other.vboID;
            other.vboID = 0;
            return *this;
        }

        void bufferData(std::vector<GLfloat> data){
            if(vboID){
                bind();
                gl::BufferData(static_cast<GLuint>(bufferType), sizeof(GLfloat) * data.size(), data.data(), static_cast<GLuint>(usage));
            }
        }

        void bind() const {
            if(vboID){
                gl::BindBuffer(static_cast<GLuint>(bufferType), vboID);
            }
        }

    private:
        GLuint vboID;
        BufferType bufferType;
        Usage usage;
    };
}