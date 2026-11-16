#pragma once

#include <glad/glad.h>
#include "types.hpp"

struct Texture2DParameters {
    GLenum wrap_s      = GL_REPEAT;
    GLenum wrap_t      = GL_REPEAT;
    GLenum min_filter  = GL_LINEAR_MIPMAP_LINEAR;
    GLenum mag_filter  = GL_LINEAR;

    bool   gen_mipmaps = true;
    bool   flip_y      = true;
};

struct Texture2D {
    glID ID;

    Texture2D(
        const char* texture_file_path,
        const Texture2DParameters& params = {}
    );

    void bind();
};