#pragma once
#include "GLid.h"

namespace gldr {

template<typename T>
class Bindable {

    class ScopedRebinder {
        ScopedRebinder& operator=(ScopedRebinder&) ;
        GLuint id;

    public:
        ScopedRebinder(GLuint rebId)
            : id(rebId)
        { }
        ~ScopedRebinder() {
            T::bindObject(id);
        }
    };

protected:
    Glid<T> id;

public:
    void bind() {
        T::bindObject(id.get());
    }

    static ScopedRebinder createRebindToCurrent() {
        return ScopedRebinder(T::getCurrentlyBound());
    }

    ScopedRebinder scopedBind() {
        auto current = T::getCurrentlyBound();
        bind();
        return ScopedRebinder(current);
    }
};

}