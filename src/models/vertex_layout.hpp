#pragma once
#include <vector>
#include <glad/glad.h>
#include "types.hpp"

static constexpr GLsizei sizeOf(GLenum type) {
    return type == GL_FLOAT         ? sizeof(float) :
           type == GL_INT           ? sizeof(int) :
           type == GL_UNSIGNED_BYTE ? sizeof(unsigned char) :
           type == GL_DOUBLE        ? sizeof(double) : 4;
}

struct VertexLayoutAttributes {
    int       qtdComponents = 0;
    GLenum    dataType      = GL_FLOAT;
    GLboolean normalize     = GL_FALSE;
    GLsizei   size          = qtdComponents * sizeOf(dataType); 
};

struct VertexLayout {
    glID ID;

    VertexLayout();

    void setLayout(
        const std::vector<VertexLayoutAttributes>& attr
    );

    void bind();
};
