#pragma once
namespace gldr {

    /*
        The Glid helper class
        It requires T to be a type which provides static
        `delete` and `create` methods.
    */

template <typename T>
class Glid {
public:
    Glid()
        : id(T::create())
    { }

    Glid(Glid<T>&& other)
        : id(other.id) {
        other.id = 0;
    }

    ~Glid() {
        T::destroy(id);
    }

    Glid<T>& operator=(Glid<T>&& other) {
        id = other.id;
        other.id = 0;
        return *this;
    }

    explicit operator GLuint() const{
        return id;
    }

private:
    GLuint id;
};

}
