#ifndef _TEXTURE_2H_
#define _TEXTURE_2H_
#include "containers/darray.h"
#include "shader.h"
#include <assimp/cimport.h>     // Plain-C interface
#include <assimp/scene.h>       // output data structures
#include <assimp/postprocess.h> // post processing flag (ie aiProcess_Triangulate)
typedef enum {
    TEXTURE_TYPE_DIFFUSE  = 0, 
    TEXTURE_TYPE_SPECULAR = 1,


    TEXTURE_TYPE_MAX
} TEXTURE_TYPE;

typedef struct texture_impl* texture2;



darray textures_from_assimp(struct aiMaterial *mat, enum aiTextureType type, TEXTURE_TYPE internal_type, const char* workdir);
void texture_to_gpu(texture2 tp);
void texture2_destroy(texture2 tp);
void texture_activate(texture2 tp, uint8_t cell, shader sh);

bool texture_is_diffuse(texture2 t);
bool texture_is_specular(texture2 t);

const char* texture_get_name(texture2 tp, uint8_t cell);

size_t texture_sizeof(void);
#endif 