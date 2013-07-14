#pragma once
#include <utility>
#include <stdexcept>
#include <vector>
#include "Glid.h"

namespace gldr {
enum class VertexBufferType : GLenum {
    DATA_BUFFER = gl::ARRAY_BUFFER,
    INDEX_BUFFER = gl::ELEMENT_ARRAY_BUFFER
};

template<VertexBufferType type = VertexBufferType::DATA_BUFFER>
class VertexBuffer {
public:
    static GLuint create() {
        GLuint id = 0;
        gl::GenFramebuffers(1, &id);
        if (!id)
            throw std::runtime_error("Problem creating a texture");
        return id;
    }

    static void destroy(GLuint id) {
        gl::DeleteFramebuffers(1, &id);
    }

    enum class Usage : GLenum {
        DYNAMIC_DRAW = gl::DYNAMIC_DRAW,
        DYNAMIC_READ = gl::DYNAMIC_READ,
        STATIC_DRAW = gl::STATIC_DRAW,
        STATIC_READ = gl::STATIC_READ,
        STREAM_DRAW = gl::STREAM_DRAW,
        STREAM_READ = gl::STREAM_READ,
    };
    
private:
    Glid<VertexBuffer> id;
    Glid<VertexBuffer> savedId;

    Usage usage;

public:
    void bind() {
        gl::BindBuffer(static_cast<GLenum>(type), id.get());
    }

    /*

    namespace detail {
    using std::begin; // fallback for ADL
    template<class C>
    auto adl_begin(C& c) -> decltype(begin(c)); // undefined, not needed
    template<class C>
    auto adl_begin(C const& c) -> decltype(begin(c)); // undefined, not needed
    }

    template<
        typename Range,
        class = typename std::enable_if<
            std::is_convertible<
                decltype(begin(std::declval<Range&>())),
                char
            >::type
        >
    >
    void data(Range r) {
        glBufferData(static_cast<GLenum>(type), size, data, static_cast<GLenum>(usage));
    }*/

    template<
        typename T,
        class = typename std::enable_if<std::is_pod<T>::value>
    >
    void data(std::vector<T> const& v) {
        // v.data() uses implicit conversion to void* here
/*#ifdef GLDR_HAS_DSA
        gl::NamedBufferDataEXT(id.get() , v.size() * sizeof(T), v.data(), static_cast<GLenum>(usage));
#else*/
        bind();
        gl::BufferData(static_cast<GLenum>(type), v.size() * sizeof(T), v.data(), static_cast<GLenum>(usage));
/*#error "not implemented properly yet"
#endif*/
    }

    VertexBuffer(Usage _usage = Usage::STATIC_DRAW) :
        usage(_usage)
    { }

    VertexBuffer(VertexBuffer&& other) :
        id(std::move(other.id)),
        savedId(std::move(other.savedId)),
        usage(other.usage),
        type(other.type)
    { }
};


#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

/*void VertexBuffer::_UpdateCachedValue() {
    int temp;
    switch (type) {
    case Type::DATA_BUFFER:
        gl::GetIntegerv(GL_ARRAY_BUFFER_BINDING, &temp);
        break;
    case Type::INDEX_BUFFER:
        gl::GetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &temp);
        break;
    }

    savedId = temp;
}*/


} // namespace gldr
