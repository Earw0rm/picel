#include "texture.h"
#include "stb_image.h"
#include "logger.h"



int8_t texture_init(const char* path, texture* tp){
    
    // because opengl y axis is flip
    stbi_set_flip_vertically_on_load(1);
    int width = 0, height = 0, /* bits per pixel */ bpp = 0;

    unsigned char* img_data = stbi_load(path, &width, &height, &bpp, 0);
    if(!img_data){
        LOG_FATAL("Cannot load texture from %s: %s", path, stbi_failure_reason());
        return TEXTURE_STATUS_BAD_FILE_READ;
    }
    glGenTextures(1, &tp->texture_obj);
    glBindTexture(GL_TEXTURE_2D, tp->texture_obj);
    // define texture image and load data to GPU
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        /* how we store inside gpu*/GL_RGB,
        width,
        height,
        0,
        /*format of stb image*/ GL_RGB,
        GL_UNSIGNED_BYTE,
        img_data
    );

    stbi_image_free(img_data);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    tp->is_initialized = true;
    return TEXTURE_STATUS_OK;
}


void texture_destroy(texture* tp){
    glDeleteTextures(1, &tp->texture_obj);
}