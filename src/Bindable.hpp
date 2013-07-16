#pragma once
#include "GLid.h"
#include <boost/optional.hpp>

namespace gldr {

template<typename T>
class ScopedRebinder {
    ScopedRebinder& operator=(ScopedRebinder&) /* = delete*/;
    ScopedRebinder(ScopedRebinder&) /* = delete */;
    boost::optional<GLuint> id;

public:
    ScopedRebinder(GLuint rebId)
        : id(rebId)
    { }
    ScopedRebinder(ScopedRebinder && rhs) {
        // workaround for lack of move constructor in boost::optional
        id.swap(rhs.id);
    }
    ~ScopedRebinder() {
        if (id)
            T::bindObject(id.get());
    }
};

template<typename T>
class Bindable {
protected:
    Glid<T> id;

public:
    void bind() {
        T::bindObject(id.get());
    }

    static ScopedRebinder<T> createRebindToCurrent() {
        return ScopedRebinder<T>(T::getCurrentlyBound());
    }

    ScopedRebinder<T> scopedBind() {
        auto current = T::getCurrentlyBound();
        bind();
        return ScopedRebinder<T>(current);
    }

    // For Visual Studio
    Bindable() {
    }
    Bindable(Bindable&& rhs) 
        : id(std::move(rhs.id)) {
    }
    Bindable& operator=(Bindable && rhs) {
        id = std::move(rhs.id);
    }
};

}