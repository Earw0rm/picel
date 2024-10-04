#include "uvn_camera.h"
#include <stdlib.h>
#include "memory.h"
#include "math.h"
#include "event_system.h"
#include <GLFW/glfw3.h>




struct camera_mat{
/**
 * Original UVM (u right, v up, m back)
 * -------------------------
 * | u v n |.T -dot(u, px) |
 * | u v n |   -dot(v, py) |
 * | u v n |   -dot(n, pz) |
 * -------------------------
 * | 0 0 0 |   1           |
 * -------------------------
 **/
    float speed;
    vector3f world_space_up;
    vector3f world_position;
    // actually pointing in the reverse direction
    vector3f direction; // v - from target to the camera
    // this is dynamic vector.
    vector3f up; // v - from camera to up
    vector3f right; // u - from camera to right
    // rotations
    float pitch; // -> around x axis
    float yaw;   // -> around y axis
    // float_t roll;  // -> around z axis

    float last_x;
    float last_y;


    // litle configuration
    bool first_mouse;
    float mouse_sensitivity;    
};



static bool // TODO refactor
on_mouse_pos_changed(event_code code, 
             void* sender,
             void* listener_inst,
             event_context data){
    camera c = (camera) listener_inst;
    double xpos = data.data.d64[0];
    double ypos = data.data.d64[1];            

    if(c->first_mouse){
        c->last_x = xpos;
        c->last_y = ypos;
        c->first_mouse = false;
    }

    float xoffset = xpos - c->last_x;
    float yoffset = ypos - c->last_y; // sinse y-coordinates range from bottom to top we need to invert this?
    
    c->yaw   = xoffset * c->mouse_sensitivity;
    c->pitch = yoffset * c->mouse_sensitivity;

    // if pitch == 90 then we will break it Gram-Schmidt_process sinse camera == world_space_up direction 
    if(c->pitch > 89.0f){
        c->pitch = 89.0f;
    }
    if(c->pitch < -89.0f){
        c->pitch = -89.0f;
    }

    vector3f direction = {
        .x = cosf(DEGREE2RADIANS(c->yaw)) * cosf(DEGREE2RADIANS(c->pitch)),
        .y = sinf(DEGREE2RADIANS(c->pitch)),
        .z = sinf(DEGREE2RADIANS(c->yaw)) * cosf(DEGREE2RADIANS(c->pitch)),
    };

    // after updating direction we need to consyst of right and up vector
    c->direction = vec3f_normalize(direction);
    c->right = vec3f_normalize(vec3f_cross(c->world_space_up, c->direction));
    c->up = vec3f_cross(c->direction, c->right);

    return false;
}

static bool 
on_key_press(event_code code, 
             void* sender,
             void* listener_inst,
             event_context data){
    camera c = (camera) listener_inst;
    uint16_t key_code = data.data.ui16[0];

    if(key_code == GLFW_KEY_W){
        c->world_position = vec3f_diff(c->world_position, vec3f_scalar_mul(c->direction, c->speed));
    }

    if(key_code == GLFW_KEY_S){
        c->world_position = vec3f_summ(c->world_position, vec3f_scalar_mul(c->direction, c->speed));
    }

    if(key_code == GLFW_KEY_A){
        c->world_position = vec3f_summ(
            c->world_position,
            vec3f_scalar_mul(vec3f_normalize(vec3f_cross(c->direction, c->up)), c->speed)
        );
    }

    if(key_code == GLFW_KEY_D){
        c->world_position = vec3f_diff(
            c->world_position,
            vec3f_scalar_mul(vec3f_normalize(vec3f_cross(c->direction, c->up)), c->speed)
        );
    }

    return false;
}

void camera_set_position(camera c, float x, float y, float z){
    c->world_position = v3f(x, y, z);   
}

camera camera_init(vector3f camera_position,
                   vector3f camera_target,
                   vector3f world_space_up /* |0, 1, 0| by default */){
    camera res = malloc(sizeof(struct camera_mat));
    //TODO crunch
    if(!event_system_register(EVENT_CODE_KEY_PRESSED, res, on_key_press)){
        free(res);
        return nullptr;
    }
    //TODO crunch again
    if(!event_system_register(EVENT_CODE_MOUSE_MOVED, res, on_mouse_pos_changed)){
        free(res);
        return nullptr;
    }

    res->world_space_up = world_space_up;
    // should be in math library?
    // https://en.wikipedia.org/wiki/Gram-Schmidt_process
    res->world_position = camera_position;
    res->direction = vec3f_normalize(vec3f_diff(camera_position, camera_target));
    res->right = vec3f_normalize(vec3f_cross(world_space_up, res->direction));
    res->up = vec3f_cross(res->direction, res->right);

    res->speed = 0.1f;
    res->last_x = 0;
    res->last_y = 0;
    //default values
    res->yaw = -90.0f;
    res->pitch = 0;
    res->mouse_sensitivity = 0.01f;
    return res;
}



void camera_destroy(camera camera){
    free(camera);
}

matrix4f camera_get_view(camera camera){

    matrix4f a = mat4f_id(1);
    matrix4f b = mat4f_id(1);
    a.m[0] = camera->right.x; 
    a.m[1] = camera->up.x;                       
    a.m[2] = camera->direction.x;

    a.m[4] = camera->right.y;
    a.m[5] = camera->up.y;                        
    a.m[6] = camera->direction.y;

    a.m[8]  = camera->right.z;
    a.m[9]  = camera->up.z;                       
    a.m[10] = camera->direction.z;

    b.m[12] = -camera->world_position.x;
    b.m[13] = -camera->world_position.y;
    b.m[14] = -camera->world_position.z;
    matrix4f res = mdotm4(a, b);
    log_matrix4f(res);
    return res;
}