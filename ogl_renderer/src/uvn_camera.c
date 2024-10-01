#include "uvn_camera.h"
#include <stdlib.h>
#include "memory.h"
#include "math.h"
struct camera_mat{
    matrix4f value;
};


camera camera_init(){
    camera res = malloc(sizeof(struct camera_mat));
    res->value = mat4f_id(1); 
    return res;
}

void camera_move(camera camera, float x, float y, float z){
    camera->value.m[12] += -x;
    camera->value.m[13] += -y;
    camera->value.m[14] += -z;
}

void camera_destroy(camera camera){
    free(camera);
}

matrix4f camera_get_data(camera camera){
    return camera->value;
}