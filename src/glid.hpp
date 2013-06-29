#pragma once
namespace gldr{
    template <void(*deletor)(GLuint &)>
    struct Glid{
        Glid(GLuint id = 0):
            id(id){}

        Glid(Glid&& other):
            id(other.id){
            other.id = 0;
        }

        ~Glid(){
            deletor(id);
        }

        operator GLuint() const{
            return id;
        }

        GLuint* ptr(){
            return &id;
        }

    private:
        GLuint id;
    };
}