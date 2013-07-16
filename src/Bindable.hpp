#pragma once
#include "GLid.h"
#include <boost/optional.hpp>

namespace gldr {

template<typename T>
class Bindable {

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