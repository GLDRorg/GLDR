#pragma once
#include <functional>
namespace gldr{
    namespace {
        void style_a(GLuint);
        void style_b(GLsizei, const GLuint*);
    }

    template <typename T>
    struct Glid{
        Glid(T deletor, GLuint id = 0):
            id(id),
            deletor(deletor){}

        Glid(Glid&& other):
            id(other.id),
            deletor(deletor){
            other.id = 0;
        }

        virtual ~Glid();

        operator GLuint() const {
            return id;
        }

        GLuint* ptr() {
            return &id;
        }

    private:
        GLuint id;
        T deletor;
    };

    template<>
    Glid<decltype(&style_a)>::~Glid(){
        deletor(id);
    }
    
    template<>
    Glid<decltype(&style_b)>::~Glid(){
        deletor(1, &id);
    }
}