#ifndef _WORLD_H_
#define _WORLD_H_

#include "ecs.h"
#include "window.h"
#include "uvn_camera.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"

// this is context manageg. Holding all possible global resources and protect them by mutex
// TODO impliment concurrent access 
void world_init(void);
void world_destroy(void);

ecs world_get_ecs(void);
camera world_get_main_camera(void);
shader world_get_basic_shader(void);
mesh world_get_basic_mesh(void);
window world_get_main_window(void);
texture world_get_basic_texture(void);
#endif 