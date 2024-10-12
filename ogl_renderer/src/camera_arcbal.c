

#include "camera.h"
#include "event_system.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define CAMERA_SPEED (0.1f)
#define MOUSE_SENSITIVITY (0.01f)
struct camera_mat{
    bool is_left_button_press;
    bool is_right_button_press;
    bool is_middle_button_press;


    bool last_position_is_initialized;
    float last_xpos;
    float last_ypos;    
};


static bool 
on_button_press(event_code code, 
             void* sender,
             void* listener_inst,
             event_context data){
    camera c = (camera) listener_inst;

    uint16_t button = data.data.ui16[0];
    uint16_t action = data.data.ui16[1];
    // uint16_t mods   = data.data.ui16[2];    

    if(action == GLFW_PRESS){
        LOG_INFO("BTN_PRESS");
        switch (button){
            case GLFW_MOUSE_BUTTON_LEFT:
                c->is_left_button_press = true;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                c->is_right_button_press = true;
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                c->is_middle_button_press = true;
                break;                
            default:
                break;
        }        
    }
    if(action == GLFW_RELEASE){
        LOG_INFO("BTN_RELEASE");
        switch (button){
            case GLFW_MOUSE_BUTTON_LEFT:
                c->is_left_button_press = false;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                c->is_right_button_press = false;
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                c->is_middle_button_press = false;
                break;                
            default:
                break;
        }    
    }    

    return false;
}

static bool // TODO refactor
on_mouse_pos_changed(event_code code, 
             void* sender,
             void* listener_inst,
             event_context data){
    // camera c = (camera) listener_inst;

    // if(!c->last_position_is_initialized){
    //     c->last_xpos = data.data.d64[0];
    //     c->last_ypos = data.data.d64[1];
    //     c->last_position_is_initialized = true;
    //     return false;
    // }

    // double delta_x = (data.data.d64[0] - c->last_xpos);
    // double delta_y = (data.data.d64[1] - c->last_ypos);

    // c->last_xpos = data.data.d64[0];
    // c->last_ypos = data.data.d64[1];


    return false;
}

static bool 
on_key_press(event_code code, 
             void* sender,
             void* listener_inst,
             event_context data){
    // camera c = (camera) listener_inst;
    // uint16_t key_code = data.data.ui16[0];
    
    // if(key_code == GLFW_KEY_W){
    //     c->position = vec3f_summ(c->position, vec3f_scalar_mul(front, CAMERA_SPEED));
    // }

    // if(key_code == GLFW_KEY_S){
    //     c->position = vec3f_diff(c->position, vec3f_scalar_mul(front, CAMERA_SPEED));
    // }

    // if(key_code == GLFW_KEY_A){
    //     c->position = vec3f_diff(c->position, vec3f_scalar_mul(right, CAMERA_SPEED));
    // }

    // if(key_code == GLFW_KEY_D){
    //     c->position = vec3f_summ(c->position, vec3f_scalar_mul(right, CAMERA_SPEED));
    // }


    return false;
}

camera camera_init(vector3f camera_position,
                   vector3f camera_target){
    camera c = malloc(sizeof(struct camera_mat));


    if(!event_system_register(EVENT_CODE_KEY_PRESSED, c, on_key_press)){
        LOG_FATAL("cannot register camera to the event bus");
        free(c);
        return nullptr;
    }

    if(!event_system_register(EVENT_CODE_BUTTON_PRESSED, c, on_button_press)){
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

  
    
    matrix4f res = mat4f_id(1);

    return res;
}

