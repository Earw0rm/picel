#ifndef _SCENE_LOADER_H_
#define _SCENE_LOADER_H_
#include "containers/darray.h"

typedef struct scene_impl* scene;

scene scene_load(const char* filepath);
darray scene_get_meshes(scene s);

size_t scene_sizeof(void);
#endif 