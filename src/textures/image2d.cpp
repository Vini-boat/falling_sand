#include "image2d.hpp"
#include <stb_image.h>
#include "logger/logger.hpp"

Image2D::Image2D(
    const char* path,
    bool flip_y
){
    _log.info(std::format("IMAGE2D: trying to load {}", path));
    stbi_set_flip_vertically_on_load(flip_y);

    data = stbi_load(
        path, &width, &height, &channels, 4
    );   
    // tratar o erro
    _log.info(std::format("IMAGE2D: loaded {}", path));
}

void Image2D::free(){
    stbi_image_free(data);
    _log.info(std::format("Image2D: free"));
}