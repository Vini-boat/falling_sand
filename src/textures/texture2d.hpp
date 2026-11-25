#pragma once

#include <glad/glad.h>
#include "types.hpp"
#include "image2d.hpp"

namespace WrapMode {
    enum WrapMode : GLenum {
        REPEAT          = GL_REPEAT,
        MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
        CLAMP_TO_EDGE   = GL_CLAMP_TO_EDGE,
        CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
    };
}
namespace FilterMode{
    enum FilterMode : GLenum {
        NEAREST                = GL_NEAREST,
        LINEAR                 = GL_LINEAR,
        NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
        LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR
    };
}


// TODO:
// namespace InternalFormat {
//     enum InternalFormat : GLenum {

//     };
// }

// namespace Format {
//     enum Format : GLenum {

//     };
// }

// namespace FormatDataType {
//     enum FormatDataType : GLenum {

//     };
// }


struct Texture2DFormat {
    GLenum internal;
    GLenum format;
    GLenum type;
};

struct Texture2DParameters {
    WrapMode::WrapMode   wrap_s        = WrapMode::REPEAT;
    WrapMode::WrapMode   wrap_t        = WrapMode::REPEAT;
    FilterMode::FilterMode min_filter  = FilterMode::LINEAR_MIPMAP_LINEAR;
    FilterMode::FilterMode mag_filter  = FilterMode::LINEAR;
    Texture2DFormat format = Texture2DFormat{
        .internal = GL_RGBA,
        .format   = GL_RGBA,
        .type     = GL_UNSIGNED_BYTE
    };
    bool       gen_mipmaps = true;
};

struct Texture2D {
    glID ID;
    Texture2DFormat format;

    Texture2D(
        const void* data,
        int width, int height,
        const Texture2DParameters& params = {}
    );
    Texture2D(
        const Image2D& img, 
        const Texture2DParameters& params = {}
    );
    
    void update(
        const void* data,
        int width, int height,
        int offset_x, int offset_y
    );

    void bind(unsigned int texture_unit);
};