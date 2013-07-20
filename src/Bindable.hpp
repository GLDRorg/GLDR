#pragma once
#include "glid.hpp"
#include <boost/optional.hpp>

namespace gldr {

template<typename T>
class ScopedBinder {
    boost::optional<GLuint> id;

public:
    ScopedBinder(GLuint rebId)
        : id(rebId)
    { }
    ScopedBinder(ScopedBinder && rhs) {
        // workaround for lack of move constructor in boost::optional
        id.swap(rhs.id);
    }
    ~ScopedBinder() {
        if (id) {
            T::bindObject(id.get());
        }
    }

private:
    ScopedBinder& operator=(ScopedBinder&) /* = delete*/;
    ScopedBinder& operator=(ScopedBinder&&) /* = delete*/;
    ScopedBinder(ScopedBinder&) /* = delete */;
};

template<typename T>
class Bindable {
protected:
    Glid<T> id;

public:
    void bind() {
        T::bindObject(id.get());
    }

    static void unbind() {
        T::bindObject(0);
    }

    static ScopedBinder<T> createRebindToCurrent() {
        return ScopedBinder<T>(T::getCurrentlyBound());
    }

    ScopedBinder<T> scopedBind() {
        auto current = T::getCurrentlyBound();
        bind();
        return ScopedBinder<T>(current);
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