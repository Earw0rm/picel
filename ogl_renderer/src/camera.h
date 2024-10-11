#ifndef _UVM_CAMERA_H_
#define _UVM_CAMERA_H_
#include "picel_math.h"

typedef struct camera_mat* camera;


camera camera_init(vector3f camera_position,
                   vector3f camera_target);

void camera_destroy(camera camera);
matrix4f camera_get_view(camera camera);



#endif 