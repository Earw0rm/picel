

#include "window.h"
#include "logger.h"
#include "camera.h"
#include "math.h"
#include "event_system.h"
#include <stdlib.h>


struct window_impl{
    const char* title;
    uint32_t height;
    uint32_t width;
    GLFWwindow* window;

};




static void 
resize_callback(GLFWwindow* window, int32_t width, int32_t height){
    glViewport(0, 0, width, height);    
}

static void
key_input_callback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods){

    if(key == GLFW_KEY_ESCAPE){
        glfwSetWindowShouldClose(window, true);
    }
}

static void
mouse_callback(GLFWwindow* window, double_t xpos, double_t ypos){
    event_context ctx;
    ctx.data.d64[0] = xpos;
    ctx.data.d64[1] = ypos;

    event_system_fire(EVENT_CODE_MOUSE_MOVED, nullptr, ctx);
}

static void 
process_camera_move(window win){
    uint32_t key_w = glfwGetKey(win->window, GLFW_KEY_W);
    uint32_t key_s = glfwGetKey(win->window, GLFW_KEY_S);
    uint32_t key_a = glfwGetKey(win->window, GLFW_KEY_A);
    uint32_t key_d = glfwGetKey(win->window, GLFW_KEY_D);

    if(key_w == GLFW_PRESS){
        event_context ctx;
        ctx.data.ui16[0] = GLFW_KEY_W; 
        event_system_fire(EVENT_CODE_KEY_PRESSED, nullptr, ctx);
    }
    if(key_s == GLFW_PRESS){
        event_context ctx;
        ctx.data.ui16[0] = GLFW_KEY_S; 
        event_system_fire(EVENT_CODE_KEY_PRESSED, nullptr, ctx);
    }
    if(key_a == GLFW_PRESS){
        event_context ctx;
        ctx.data.ui16[0] = GLFW_KEY_A; 
        event_system_fire(EVENT_CODE_KEY_PRESSED, nullptr, ctx);
    }
    if(key_d == GLFW_PRESS){
        event_context ctx;
        ctx.data.ui16[0] = GLFW_KEY_D; 
        event_system_fire(EVENT_CODE_KEY_PRESSED, nullptr, ctx);
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

    glfwSetFramebufferSizeCallback(win->window, resize_callback);
    glfwSetKeyCallback(win->window, key_input_callback);
 
    win->height = height;
    win->width  = width;

    /**
     * Hide the cursor and capture it.
     * Capturing a cursor means that, once the application has focus, 
     * the mouse cursor stays within the center of the window 
     * (unless the application loses focus or quits). We can do
     * this with one simple configuration call:
     */
    glfwSetInputMode(win->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
    process_camera_move(win);
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