#pragma once
#include <GL/glew.h>
#include <functional>

namespace gldr {

// Safe-moving non-copyable OpenGL id wrapper

class GLId { 
    typedef std::function<void(GLuint)> Deleter;
    Deleter deleter;
    GLuint innerId;

    // =delete commented out due to MSVS support
    GLId(GLId const&) /* = delete*/;
    GLId& operator=(GLId const&) /* = delete*/;

    // doesn't work in msvs god knows why
   /* template<typename Deleter_t>
    static auto call_deleter(Deleter_t& del, GLuint handle) -> decltype(del(handle)) {
        return del(handle);
    }

    template<typename Deleter_t>
    static auto call_deleter(Deleter_t& del, GLuint handle) -> decltype(del(1, &handle)) {
        return del(1, &handle);
    }*/

public:
    operator GLuint () const { return innerId; }

    // meant to be used with glGenXs and glDeleteXs functions
    const GLuint* const get_ptr() const { return &innerId; }
    GLuint* get_ptr() { return &innerId; }

    GLId(Deleter del) 
        : innerId(0)
        , deleter(std::move(del))
    { }
    
    GLId(GLuint id, Deleter del)
        : innerId(id)
        , deleter(std::move(del))
    { }

    GLId(GLId&& other) {
        innerId = other.innerId;
        other.innerId = 0;
    }

    GLId& operator= (GLId&& other) {
        deleter(innerId);
        innerId = other.innerId;
        other.innerId = 0;
        return *this;
    }

    GLId& operator= (GLuint id) {
        deleter(innerId);
        innerId = id;
        return *this;
    }

    ~GLId() {
        //call_deleter(deleter, innerId);
        deleter(innerId);
    }

    bool operator== (GLuint b) const {
        return innerId == b;
    }
    bool operator!= (GLuint b) const {
        return !((*this) == b);
    }

    bool operator== (GLId const& b) const {
        return innerId == b.innerId;
    }
    bool operator!= (GLId const& b) const {
        return !((*this) == b);
    }
};

}