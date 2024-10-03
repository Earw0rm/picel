#ifndef _UVM_CAMERA_H_
#define _UVM_CAMERA_H_
#include "math_struct.h"

typedef struct camera_mat* camera;


camera camera_init();
void camera_move(camera camera, float x, float y, float z);
void camera_destroy(camera camera);
matrix4f camera_get_view(camera camera);
void camera_rotate(camera camera, float ax, float by, float cz);

#endif 