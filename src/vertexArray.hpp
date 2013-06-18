#pragma once
namespace gldr{
    struct VertexArray{
        VertexArray(){
            gl::GenVertexArrays(1, &vaoID);
        }

        VertexArray(VertexArray&& other):
            vaoID(other.vaoID){
            other.vaoID = 0;
        }

        ~VertexArray(){
            if(vaoID){
                gl::DeleteVertexArrays(1, &vaoID);
            }
        }

        VertexArray& operator= (VertexArray&& other) {
            vaoID = other.vaoID;
            other.vaoID = 0;
            return *this;
        }

        void bind() const {
            if(vaoID){
                gl::BindVertexArray(vaoID);
            }
        }

    private:
        GLuint vaoID;
    };
}