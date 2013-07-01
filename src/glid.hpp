#pragma once
namespace gldr{
    template <typename T>
    struct Glid{
        Glid():
            id(T::creater()){}

        Glid(Glid&& other):
            id(other.id){
            other.id = 0;
        }

        ~Glid(){
            T::deleter(id);
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