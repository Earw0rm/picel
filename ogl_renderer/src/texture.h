#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <glad/glad.h>

typedef struct texture{
    GLuint texture_obj;
    bool is_initialized;
} texture;


enum texture_status{
    TEXTURE_STATUS_BAD_FILE_READ              = -1,
    TEXTURE_STATUS_OK                         =  0,
    TEXTURE_STATUS_MAX_VALUE
};

int8_t texture_init(const char* path, texture* tp);


void texture_destroy(texture* tp);




#endif 