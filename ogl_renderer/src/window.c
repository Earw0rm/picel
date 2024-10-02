#include "window.h"
#include "logger.h"
#include <stdlib.h>
#include "uvn_camera.h"

static struct window{
    const char* title;
    uint32_t height;
    uint32_t width;
    GLFWwindow* window;
    bool is_initialized;
    camera main_camera;
} win;

static void 
resize_callback(GLFWwindow* window, int32_t width, int32_t height){
    glViewport(0, 0, width, height);    
}

static void
key_input_callback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods){
    LOG_INFO("KeyPress. Key: %i, Scancode: %i, action: %i, mods: %i", 
             key, scancode, action, mods);

    if(key == GLFW_KEY_ESCAPE){
        glfwSetWindowShouldClose(window, true);
    }
}


// TODO tmp
void 
win_process_camera_move(){
    uint32_t key_w = glfwGetKey(win.window, GLFW_KEY_W);
    uint32_t key_s = glfwGetKey(win.window, GLFW_KEY_S);
    uint32_t key_a = glfwGetKey(win.window, GLFW_KEY_A);
    uint32_t key_d = glfwGetKey(win.window, GLFW_KEY_D);

    if(key_w == GLFW_PRESS){
        camera_move(win.main_camera, 0.0, -0.1, 0.0);
    }
    if(key_s == GLFW_PRESS){
        camera_move(win.main_camera, 0.0, 0.1, 0.0);
    }
    if(key_a == GLFW_PRESS){
        camera_move(win.main_camera, 0.1, 0.0, 0.0);
    }
    if(key_d == GLFW_PRESS){
        camera_move(win.main_camera, -0.1, 0.0, 0.0);
    }
}


uint8_t 
win_init(const char* title, uint32_t height, uint32_t width){
    if(win.is_initialized){
        return BAD_DOUBLE_INITIALIZATION;
    }
    if(!glfwInit()){
        LOG_FATAL("cannot initialize glfw");
        return BAD_GLFW_INIT;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if((win.window = glfwCreateWindow(width, height, title, nullptr, nullptr)) == nullptr){
        LOG_FATAL("cannot create glfw window");
        return BAD_GLFW_WINDOW_CREATE;
    }

    glfwMakeContextCurrent(win.window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
         LOG_FATAL("cannot load glad");
         return BAD_GLAD_LOAD;
    }

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    glfwSetFramebufferSizeCallback(win.window, resize_callback);
    glfwSetKeyCallback(win.window, key_input_callback);
    win.height = height;
    win.width  = width;
    // TODO CRUNCH
    win.main_camera = camera_init();

    win.is_initialized = true;
    LOG_INFO("Window initialized");
    return OK;
}

void 
win_destroy(){
    if(win.window == nullptr){
        LOG_DEBUG("cannot destroy nullptr window");
        return;
    }
    if(!win.is_initialized){
        LOG_DEBUG("cannot destroy not initialized window");
        return;        
    }
    camera_destroy(win.main_camera);
    glfwDestroyWindow(win.window);
    glfwTerminate();
}

bool
win_should_close(){
    if(!win.is_initialized){
        LOG_DEBUG("calling win_should_close without initialization");
        return true;
    }
    return glfwWindowShouldClose(win.window);
}

void 
win_swap_buffers(){
    if(!win.is_initialized) {
        LOG_DEBUG("calling swapBuffers without initialization");
        return;
    }
    glfwSwapBuffers(win.window);
}
void
win_poll_events(){
    if(!win.is_initialized){
        LOG_DEBUG("calling pollEvents without initialization");
        return;
    }
    glfwPollEvents();
    // TODO also hire we need to check LIVE_KEYS
    // win_process_camera_move now
}

float
win_get_aspect_ratio(){
    return (((float) win.width) / ((float)win.height));
}
matrix4f win_get_view(){
    return camera_get_data(win.main_camera);
}