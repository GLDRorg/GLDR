#pragma once
#include "GLid.h"

template<typename T>
class Bindable {
protected:
    gldr::Glid<T> id;

    void forceBind() { 
    };

    void bind() {
        forceBind();
    }

};

template<typename T>
class LazyBindable : protected Bindable<T> {
protected:
    GLuint savedId;

    unsigned getCurrentlyBound() {
    }

    // "lazy" bind
    void bind() {
        if ((savedId = getCurrentlyBound()) != id.get()) {
            forceBind();
            // this is here because of "don't pay for what you don't use"
            // involved in forceBind
            savedId = id.get();
        }
    }
};

