#pragma once
#include <vector>
#include <glad/glad.h>
#include "types.hpp"

namespace VertexDataType {
    enum Type : GLenum {
        FLOAT         = GL_FLOAT,
        INT           = GL_INT,
        UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
        DOUBLE        = GL_DOUBLE
    };
};


static constexpr GLsizei sizeOf(VertexDataType::Type type) {
    return type == VertexDataType::FLOAT         ? sizeof(float) :
        type == VertexDataType::INT           ? sizeof(int) :
        type == VertexDataType::UNSIGNED_BYTE ? sizeof(unsigned char) :
        type == VertexDataType::DOUBLE        ? sizeof(double) : 4;
};
    
struct VertexLayoutAttributes {
    int                  qtdComponents = 0;
    VertexDataType::Type dataType      = VertexDataType::FLOAT;
    GLboolean            normalize     = GL_FALSE;
    GLsizei              size          = qtdComponents * sizeOf(dataType); 
};


struct VertexLayout {
    glID ID;

    VertexLayout();

    void setLayout(
        const std::vector<VertexLayoutAttributes>& attr
    );

    void bind();
};
