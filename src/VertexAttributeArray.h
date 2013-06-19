#pragma once
#include <utility>
#include "GLId.h"

namespace gldr {

class VertexAttributeArray
{
private:
    GLId m_Id;

public:
    void Bind();
    void Draw(int startIx, int endIx);
    void EnableAttributeArray(unsigned index);

    static unsigned GetMaxVertexAttributes();

    VertexAttributeArray();
    VertexAttributeArray(VertexAttributeArray&& other) :
        m_Id(std::move(other.m_Id))
    { }
    ~VertexAttributeArray(void);
};

} // namespace gldr