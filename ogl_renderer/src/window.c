#include "window.h"
#include "logger.h"
#include <stdlib.h>


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



uint8_t 
win_init(const char* title, uint32_t height, uint32_t width, window* win){
    if(win == NULL) {
       LOG_FATAL("calling win_init without actualy allocate win variable");
       return BAD_PTR;
    }

    if(!glfwInit()){
        LOG_FATAL("cannot initialize glfw");
        return BAD_GLFW_INIT;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if((win->window = glfwCreateWindow(height, width, title, NULL, NULL)) == NULL){
        LOG_FATAL("cannot create glfw window");
        return BAD_GLFW_WINDOW_CREATE;
    }

    glfwMakeContextCurrent(win->window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
         LOG_FATAL("cannot load glad");
         return BAD_GLAD_LOAD;
    }

    glEnable(GL_DEPTH_TEST);

    // glEnable(GL_CULL_FACE);
    // glFrontFace(GL_CW);
    // glCullFace(GL_BACK);

    glfwSetFramebufferSizeCallback(win->window, resize_callback);
    glfwSetKeyCallback(win->window, key_input_callback);
    
    win->is_initialized = true;
    LOG_INFO("Window initialized");
    return OK;
}

void 
win_destroy(window* win){
    if(win->window == NULL){
        LOG_DEBUG("cannot destroy nullptr window");
        return;
    }
    if(!win->is_initialized){
        LOG_DEBUG("cannot destroy not initialized window");
        return;        
    }
    glfwDestroyWindow(win->window);
    glfwTerminate();
}

bool
win_should_close(window* win){
    if(!win->is_initialized){
        LOG_DEBUG("calling win_should_close without initialization");
        return true;
    }
    return glfwWindowShouldClose(win->window);
}

void 
win_swap_buffers(window* win){
    if(!win->is_initialized) {
        LOG_DEBUG("calling swapBuffers without initialization");
        return;
    }
    glfwSwapBuffers(win->window);
}
void
win_poll_events(window* win){
    if(!win->is_initialized){
        LOG_DEBUG("calling pollEvents without initialization");
        return;
    }
    glfwPollEvents();
}