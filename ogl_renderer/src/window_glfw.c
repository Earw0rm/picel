

#include "window.h"
#include "logger.h"
#include "camera.h"
#include "math.h"
#include "event_system.h"
#include <stdlib.h>

#define ROTATION_SENSETIVE 1

/**
 * 
 * 
 * ArcballCamera rotation algorithm
 * 
 * 1) Transform start and current positions to unit vectors.
 * 2) Get rotational axis by taking cross product of start and current positions and transform it to unit vector.
 * 3) Get cosine of the rotation angle by taking dot product of start and current unit vectors.
 * 4) Get rotation angle θ.
 * 5) Build currentQuaternion struct with θ and rotational axis by using required operations (assign the cosine of half the angle and rotational axis which is scaled by sine of half the angle).
 * 6) Take q′=qcurrent*qlast product.
 * 7) Replace lastQuaternion variables with new variables after the mouse button is released.
 * 
 * 
 * 
 * Notes
 * Radius of the sphere related to the coordinate system. If our radius is equal to 1, then we use NDC.
 * If our radius in equal to K, then we use NDC * K.
 * 
 * xndc = (2*xpos/width) - 1
 * yndc = 1 - (2*ypos/height)
 */
//singleton window camera. Boolsheet and crunch
static struct arcball_camera{
    vector3f camera_direction;
    vector3f camera_up;
    vector3f camera_position;

    vector3f rotation_start_position;
    bool is_rotating_now;
    quaternion last_rotation;
    quaternion prev_rotation;
    
    uint32_t global_width;
    uint32_t global_height;
    float radius;

} arcball_camera;



struct window_impl{
    const char* title;
    uint32_t height;
    uint32_t width;
    GLFWwindow* window;
};



inline static float 
x_to_ndc(uint32_t global_width, double x, float radius){
    return ((2 * x / global_width) - 1) * radius;
} 

inline static float
y_to_ndc(uint32_t global_height, double y, float radius){
    return (1 - 2 * ( y / global_height)) * radius;
} 


static void 
resize_callback(GLFWwindow* window, int32_t width, int32_t height){
    glViewport(0, 0, width, height);    
    arcball_camera.global_height = height;
    arcball_camera.global_width  = width;
}

static void
key_input_callback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods){
    if(key == GLFW_KEY_ESCAPE){
        glfwSetWindowShouldClose(window, true);
    }


}

static void 
scroll_callback(GLFWwindow *window, double xoffset, double yoffset){

}
static void 
calculating_start_position(double_t xpos, double_t ypos){
    vector3f start_position;
    start_position.x = x_to_ndc(arcball_camera.global_width, xpos, arcball_camera.radius);
    start_position.y = y_to_ndc(arcball_camera.global_height, ypos, arcball_camera.radius);
    float start_pos_x_sq = start_position.x * start_position.x;
    float start_pos_y_sq = start_position.y * start_position.y;
    float radius_sq = arcball_camera.radius * arcball_camera.radius;
    if(start_pos_x_sq + start_pos_y_sq <= radius_sq){
        start_position.z = sqrtf(radius_sq - start_pos_x_sq - start_pos_y_sq);
    }else{
        start_position.z = 0;
    }
    arcball_camera.rotation_start_position = vec3f_normalize(start_position);
}

static void
mouse_callback(GLFWwindow* window, double_t xpos, double_t ypos){
    
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        if(!arcball_camera.is_rotating_now){
            calculating_start_position(xpos, ypos);
            arcball_camera.is_rotating_now = true;
            return;
        } //else, rotation is start 

        vector3f immidiate_position;
        immidiate_position.x = x_to_ndc(arcball_camera.global_width, xpos, arcball_camera.radius);
        immidiate_position.y = y_to_ndc(arcball_camera.global_height, ypos, arcball_camera.radius);

        float im_pos_x_sq = immidiate_position.x * immidiate_position.x;
        float im_pos_y_sq = immidiate_position.y * immidiate_position.y;
        float radius_sq = arcball_camera.radius * arcball_camera.radius;

        if(im_pos_x_sq + im_pos_y_sq <= radius_sq){
            immidiate_position.z = sqrtf(radius_sq - im_pos_x_sq - im_pos_y_sq);
        }else{
            immidiate_position.z = 0;
        }
        immidiate_position = vec3f_normalize(immidiate_position);   
        vector3f rotation_axis = vec3f_normalize(vec3f_cross(arcball_camera.rotation_start_position, immidiate_position));
        
        float cos_angle = vec3f_dot(immidiate_position, arcball_camera.rotation_start_position);

        if(cos_angle > 1){
            cos_angle = 1;
        }else if(cos_angle < -1){
            cos_angle = -1;
        }
        float angle = acosf(cos_angle) * ROTATION_SENSETIVE;
        quaternion quat = quaternion_from_axis_angle(angle, rotation_axis.x, -rotation_axis.y, rotation_axis.z);
        

        // arcball_camera.last_rotation = quaternion_normalize(quaternion_mul(quat, arcball_camera.last_rotation));
        arcball_camera.last_rotation = quaternion_normalize(quaternion_mul(quat, arcball_camera.prev_rotation));
    }



    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
        if(arcball_camera.is_rotating_now){
            arcball_camera.is_rotating_now = false;
            arcball_camera.prev_rotation = arcball_camera.last_rotation;
        }

    }
}



static void error_callback(int e, const char *d){
    LOG_FATAL("GLFW ERROR: %d: %s",  e, d);
}


window
win_init(const char* title, uint32_t height, uint32_t width){

    window win = malloc(sizeof(struct window_impl));;
    glfwSetErrorCallback(error_callback);

    if(!glfwInit()){
        LOG_FATAL("cannot initialize glfw");
        goto ret_error;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if((win->window = glfwCreateWindow(width, height, title, nullptr, nullptr)) == nullptr){
        LOG_FATAL("cannot create glfw window");
        goto ret_error;
    }

    glfwMakeContextCurrent(win->window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        LOG_FATAL("cannot load glad");
        goto ret_error;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    win->height = height;
    win->width  = width;

    arcball_camera.radius = 10;
    arcball_camera.global_height = height;
    arcball_camera.global_width  = width;


    arcball_camera.camera_position = v3f(0, 0, arcball_camera.radius);
    // we look at 0
    arcball_camera.camera_direction = vec3f_diff(v3f(0, 0, 0), arcball_camera.camera_position);
    arcball_camera.camera_up = v3f(0, 1, 0);

    arcball_camera.last_rotation = quaternion_id();
    arcball_camera.prev_rotation = quaternion_id();


    glfwSetFramebufferSizeCallback(win->window, resize_callback);
    glfwSetKeyCallback(win->window, key_input_callback);

    glfwSetScrollCallback(win->window, scroll_callback);

    /**
     * Hide the cursor and capture it.
     * Capturing a cursor means that, once the application has focus, 
     * the mouse cursor stays within the center of the window 
     * (unless the application loses focus or quits). We can do
     * this with one simple configuration call:
     */
    // glfwSetInputMode(win->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glfwSetMouseButtonCallback(win->window, mbc);
    // glfwSetWindowFocusCallback(win->window, window_focus_callback);

    glfwSetCursorPosCallback(win->window, mouse_callback);  


    LOG_INFO("Window initialized");

    return win;

ret_error:
    free(win);
    return nullptr;    
}

void 
win_destroy(window win){
    if(win->window == nullptr){
        LOG_DEBUG("cannot destroy nullptr window");
        return;
    }
    
    glfwDestroyWindow(win->window);
    glfwTerminate();
    free(win);
}

bool
win_should_close(window win){
    if(win == nullptr){
        LOG_DEBUG("calling win_should_close on nullptr window");
        return true;
    }
    return glfwWindowShouldClose(win->window);
}

void 
win_swap_buffers(window win){
    if(win == nullptr) {
        LOG_DEBUG("calling swapBuffers on nullptr window");
        return;
    }
    glfwSwapBuffers(win->window);
}

void
win_poll_events(window win){
    if(win == nullptr){
        LOG_DEBUG("calling win_poll_events on nullptr window");
        return;
    }

    glfwPollEvents();
    // process_camera_move(win);
}

float
win_get_aspect_ratio(window win){
    if(win == nullptr){
        LOG_DEBUG("calling win_get_aspect_ratio on nullptr window");
        return 0;
    }
    return (((float) win->width) / ((float)win->height));
}

GLFWwindow*
win_get_glfw_ctx(window p){
    return p->window;
}

matrix4f 
win_get_view(window pwindow){
    matrix3f rotation = quaternion_to_matrix(arcball_camera.last_rotation);
    matrix4f rot4 = mat4f_from3f(rotation);

    // vector3f direction = mdotv3(rotation, arcball_camera.camera_direction);
    // vector3f up = mdotv3(rotation, arcball_camera.camera_up);
    // vector3f position = mdotv3(rotation, arcball_camera.camera_position);
    
    matrix4f res = look_at(arcball_camera.camera_position,
                           vec3f_summ(arcball_camera.camera_direction, arcball_camera.camera_position),
                           arcball_camera.camera_up);


    return mdotm4(res, rot4);
}