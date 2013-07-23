#pragma once

#include <glload/gl_4_3.hpp>

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

    Glid(Glid && other)
        : id(other.id) {
            other.id = 0;
    }

    ~Glid() {
        T::destroy(id);
    }

    Glid& operator=(Glid && other) {
        T::destroy(id);
        id = other.id;
        other.id = 0;
        return *this;
    }

    GLuint get() const {
        return id;
    }

private:
    GLuint id;

    // Required by Visual Studio to make it truly noncopyable
    Glid(Glid&) /* = delete*/;
    Glid& operator= (Glid&) /* = delete*/;
};

}
