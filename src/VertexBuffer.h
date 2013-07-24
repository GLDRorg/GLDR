#pragma once
#include <utility>
#include <stdexcept>
#include <vector>
#include <array>
#include <memory>
#include <type_traits>

#include "Bindable.hpp"

#define GLDR_HAS_DSA

namespace gldr {

    namespace detail {
        template<class T> struct is_contiguous_range : std::false_type{};
        template<class T, size_t N> struct is_contiguous_range<T[N]> : std::true_type{};
        template<class T, size_t N> struct is_contiguous_range<std::array<T, N>> : std::true_type{};
        template<class T, class Alloc> struct is_contiguous_range<std::vector<T, Alloc>> : std::true_type{};
        // vector<bool> is broken.
        template<class Alloc> struct is_contiguous_range<std::vector<bool, Alloc>> : std::false_type{};


        template<class T> struct value_type { typedef typename T::value_type type; };
        template<class T, size_t N> struct value_type<T[N]> { typedef T type; };

        template<class T> auto data(T const& x) -> decltype(std::addressof(*std::begin(x))) { return std::addressof(*std::begin(x)); }
        template<class T> auto size(T const& x) -> decltype(std::end(x) - std::begin(x)) { return std::end(x) - std::begin(x); }
    }

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

    template <
        typename Container,
        class = typename std::enable_if<detail::is_contiguous_range<Container>::value>::type
    >
    void data(Container const& v) {
        // v.data() uses implicit conversion to void* here
        typedef typename detail::value_type<Container>::type Elem;
        static_assert(std::is_pod<Elem>::value, "Element type must be a POD");

    #ifdef GLDR_HAS_DSA
        gl::NamedBufferDataEXT(this->id.get(), detail::size(v) * sizeof(Elem), detail::data(v), static_cast<GLenum>(usage));
    #else
        this->bind();
        gl::BufferData(static_cast<GLenum>(type), detail::size(v) * sizeof(Elem), detail::data(v), static_cast<GLenum>(usage));
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
