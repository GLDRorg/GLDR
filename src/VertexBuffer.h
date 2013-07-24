#pragma once
#include <utility>
#include <stdexcept>
#include <vector>

#include "Bindable.hpp"

#define GLDR_HAS_DSA

namespace gldr {
enum class VertexBufferType : GLenum {
    DATA_BUFFER = gl::ARRAY_BUFFER,
    ARRAY_BUFFER = gl::ARRAY_BUFFER,
    INDEX_BUFFER = gl::ELEMENT_ARRAY_BUFFER,
    ELEMENT_ARRAY_BUFFER = gl::ELEMENT_ARRAY_BUFFER
};

template<VertexBufferType type = VertexBufferType::DATA_BUFFER>
class VertexBuffer : public Bindable<VertexBuffer<type>> {
public:
    static GLuint create() {
        GLuint id = 0;
        gl::GenBuffers(1, &id);
        if (!id)
            throw std::runtime_error("Problem creating a texture");
        return id;
    }

    static void destroy(GLuint id) {
        gl::DeleteBuffers(1, &id);
    }

    static void bindObject(GLuint id) {
        gl::BindBuffer(static_cast<GLenum>(type), id);
    }

    static GLuint getCurrentlyBound() {
        GLint current;
        // this switch will be done in compile time
        switch (type) {
            case VertexBufferType::ARRAY_BUFFER:
                gl::GetIntegerv(gl::ARRAY_BUFFER_BINDING, &current);
                break;
            case VertexBufferType::ELEMENT_ARRAY_BUFFER:
                gl::GetIntegerv(gl::ELEMENT_ARRAY_BUFFER_BINDING, &current);
                break;
        }
        
        return current;
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
    Usage usage;

public:
    friend class VertexAttributeArray;
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
    #ifdef GLDR_HAS_DSA
        gl::NamedBufferDataEXT(this->id.get() , v.size() * sizeof(T), v.data(), static_cast<GLenum>(usage));
    #else
        this->bind();
        gl::BufferData(static_cast<GLenum>(type), v.size() * sizeof(T), v.data(), static_cast<GLenum>(usage));
    #endif
    }

    VertexBuffer(Usage _usage = Usage::STATIC_DRAW) :
        usage(_usage)
    { }

    VertexBuffer(VertexBuffer&& other)
        : Bindable(std::move(other))
    {
        usage = other.usage;
    }
    VertexBuffer& operator=(VertexBuffer&& other) {
        Bindable::operator=(std::move(other));
        usage = other.usage;
    }
};


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
