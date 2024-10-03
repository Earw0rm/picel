#ifndef _UVM_CAMERA_H_
#define _UVM_CAMERA_H_
#include "math_struct.h"

typedef struct camera_mat* camera;


camera camera_init(vector3f camera_position,
                   vector3f camera_target,
                   vector3f world_space_up /* |0, 1, 0| by default */);

void camera_destroy(camera camera);
matrix4f camera_get_view(camera camera);

void camera_set_position(camera c, float x, float y, float z);

void camera_rotate(camera camera, float ax, float by, float cz);

#endif 