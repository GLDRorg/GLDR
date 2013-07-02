#pragma once
namespace gldr{
template <typename T>
struct Glid{
    Glid():
        id(T::create()){}

    Glid(Glid&& other):
        id(other.id){
        other.id = 0;
    }

    ~Glid(){
        T::destroy(id);
    }

    glid<T>& operator=(gldr<T>&& other){
        id = other.id;
        other.id = 0;
        return *this;
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
