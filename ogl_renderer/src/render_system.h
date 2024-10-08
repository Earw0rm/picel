#ifndef _RENDER_SYSTEM_H_
#define _RENDER_SYSTEM_H_

#include "ecs.h"
#include "world.h"
#include "window.h"


void render_system_render(ecs ecs, window w, camera main_camera);

#endif 