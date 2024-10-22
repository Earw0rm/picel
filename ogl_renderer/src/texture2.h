#ifndef _TEXTURE_2H_
#define _TEXTURE_2H_
#include "darray.h"

typedef enum {
    TEXTURE_TYPE_DIFFUSE  = 0, 
    TEXTURE_TYPE_SPECULAR = 1,


    TEXTURE_TYPE_MAX
} TEXTURE_TYPE;

typedef struct texture {
    uint32_t id;
    TEXTURE_TYPE type;
    darray texture_data;
    char* path; // for comparing with another textures
} texture;

#endif 