#include "camera.h"
#include "event_system.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define CAMERA_SPEED (0.1f)
#define MOUSE_SENSITIVITY (0.01f)
struct camera_mat{
    vector3f position;
    quaternion orientation;
    quaternion initial_target;
    

    float yaw;
    float pitch;

    bool last_position_is_initialized;
    float last_xpos;
    float last_ypos;    
};


static bool 
on_key_press(event_code code, 
             void* sender,
             void* listener_inst,
             event_context data){
    camera c = (camera) listener_inst;
    uint16_t key_code = data.data.ui16[0];
    

    vector3f front = vec3f_normalize(quaternion_point_rotate(c->orientation, v3f(0, 0, -1)));
    vector3f right = vec3f_normalize(quaternion_point_rotate(c->orientation, v3f(1, 0, 0)));

    if(key_code == GLFW_KEY_W){
        c->position = vec3f_summ(c->position, vec3f_scalar_mul(front, CAMERA_SPEED));
    }

    if(key_code == GLFW_KEY_S){
        c->position = vec3f_diff(c->position, vec3f_scalar_mul(front, CAMERA_SPEED));
    }

    if(key_code == GLFW_KEY_A){
        c->position = vec3f_diff(c->position, vec3f_scalar_mul(right, CAMERA_SPEED));
    }

    if(key_code == GLFW_KEY_D){
        c->position = vec3f_summ(c->position, vec3f_scalar_mul(right, CAMERA_SPEED));
    }


    return false;
}

static bool // TODO refactor
on_mouse_pos_changed(event_code code, 
             void* sender,
             void* listener_inst,
             event_context data){
    camera c = (camera) listener_inst;

    if(!c->last_position_is_initialized){
        c->last_xpos = data.data.d64[0];
        c->last_ypos = data.data.d64[1];
        c->last_position_is_initialized = true;
        return false;
    }

    double delta_x = (data.data.d64[0] - c->last_xpos);
    double delta_y = (data.data.d64[1] - c->last_ypos);

    c->last_xpos = data.data.d64[0];
    c->last_ypos = data.data.d64[1];

    c->yaw   += MOUSE_SENSITIVITY * (delta_x);
    c->pitch += MOUSE_SENSITIVITY * (delta_y); // sinse y-coordinates range from bottom to top we need to invert this?

    // // if pitch == 90 then we will break it Gram-Schmidt_process sinse camera == world_space_up direction 
    // if(c->pitch > 89.0f){
    //     c->pitch = 89.0f;
    // }
    // if(c->pitch < -89.0f){
    //     c->pitch = -89.0f;
    // }

    quaternion araound_y = quaternion_from_axis_angle(DEGREE2RADIANS(c->yaw), 0, 1, 0);
    quaternion araound_x = quaternion_from_axis_angle(DEGREE2RADIANS(c->pitch),  1, 0, 0);
    c->orientation = quaternion_mul(araound_y, araound_x);

    return false;
}


camera camera_init(vector3f camera_position,
                   vector3f camera_target){
    camera c = malloc(sizeof(struct camera_mat));

    vector3f direction = vec3f_normalize(vec3f_diff(camera_position, camera_target));
    float yaw = atan2(direction.x, direction.z);
    float pitch = asin(direction.y);

    c->yaw = RADIANS2DEGREE(yaw);
    c->pitch = RADIANS2DEGREE(pitch);

    quaternion around_y = quaternion_from_axis_angle(yaw, 0, 1, 0);
    quaternion around_x = quaternion_from_axis_angle(pitch, 1, 0, 0);
    c->orientation = quaternion_mul(around_y, around_x);

    c->position = camera_position;
    c->initial_target = quaternion_from_vec3f(camera_target);


    c->last_position_is_initialized = false;

    if(!event_system_register(EVENT_CODE_KEY_PRESSED, c, on_key_press)){
        LOG_FATAL("cannot register camera to the event bus");
        free(c);
        return nullptr;
    }

    if(!event_system_register(EVENT_CODE_MOUSE_MOVED, c, on_mouse_pos_changed)){
        LOG_FATAL("cannot register camera to the event bus");
        free(c);
        return nullptr;
    }

    return c;
}

void camera_destroy(camera camera){
    free(camera);
}

matrix4f camera_get_view(camera camera){

    LOG_INFO("initial quartenion %f %f %f", camera->orientation.x, camera->orientation.y, camera->orientation.z);
    
    matrix4f res = quartenion_look_at(camera->position, quartenion_normalize(camera->orientation));


    return res;
}
