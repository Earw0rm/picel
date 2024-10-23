#ifndef _TEXTURE_2H_
#define _TEXTURE_2H_
#include "containers/darray.h"

struct texture_impl;
typedef struct texture_impl* texture;



darray textures_from_assimp(struct aiMaterial *mat, aiTextureType type, TEXTURE_TYPE internal_type, const char* workdir);
void texture_to_gpu(texture tp);
void texture_destroy(texture tp);
void texture_activate(texture tp, uint8_t cell, shader sh);

bool texture_is_diffuse(texture t);
bool texture_is_specular(texture t);

const char* texture_get_name(texture tp, uint8_t cell)
#endif 