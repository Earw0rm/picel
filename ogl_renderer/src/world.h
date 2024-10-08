#ifndef _WORLD_H_
#define _WORLD_H_

#include "ecs.h"
#include "window.h"
#include "uvn_camera.h"
#include "shader.h"
#include "mesh.h"


// this is context manageg. Holding all possible global resources and protect them by mutex
// TODO impliment concurrent access 
void world_init();
void world_destroy();

ecs world_get_ecs();
camera world_get_main_camera();
shader world_get_basic_shader();
mesh world_get_basic_mesh();
window world_get_main_window();
#endif 