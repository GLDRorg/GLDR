#pragma once
#include <functional>
namespace gldr{
    struct Glid{
        Glid(GLuint id, std::function<void(GLuint)> deletor):
            id(id),
            deletor(deletor){}

        Glid(std::function<void(GLuint)> deletor):
            id(0),
            deletor(deletor){}

        Glid(Glid&& other):
            id(other.id),
            deletor(deletor){
            other.id = 0;
        }

        ~Glid(){
            if(id){
                deletor(id);
            }
        }

        GLuint get() const {
            return id;
        }

    private:
        GLuint id;
        std::function<void(GLuint)> deletor;
    };
}