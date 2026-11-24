#include "image2d.hpp"
#include <stb_image.h>

Image2D::Image2D(
    const char* path,
    bool flip_y
){
    stbi_set_flip_vertically_on_load(flip_y);
    
    data = stbi_load(
        path, &width, &height, &channels, 4
    );   
    // tratar o erro
}

void Image2D::free(){
    stbi_image_free(data);
}