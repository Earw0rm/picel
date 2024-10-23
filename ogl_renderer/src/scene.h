#ifndef _SCENE_LOADER_H_
#define _SCENE_LOADER_H_
#include "darray.h"

struct scene_empl;
typedef struct scene_impl* scene;

scene scene_load(const char* filepath);
darray scene_get_meshes(scene s);
#endif 