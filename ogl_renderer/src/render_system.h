#ifndef _RENDER_SYSTEM_H_
#define _RENDER_SYSTEM_H_

#include "ecs.h"
#include "world.h"
#include "window.h"


void render_system_render(ecs ecs, window w, camera main_camera, shader gs, GLuint dvao);
void render_system_render2(scene scene, window w, camera main_camera, shader gs);
#endif 