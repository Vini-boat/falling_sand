#pragma once


struct Image2D {
    unsigned char* data;
    int width, height;
    int channels;

    Image2D(
        const char* path,
        bool flip_y = true
    );

    void free();
};