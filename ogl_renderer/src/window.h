#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <stdint.h>
#include <stdbool.h>

typedef struct window{
    const char* title;
    uint32_t height;
    uint32_t width;
    GLFWwindow* window;
    bool is_initialized;
}window;

enum WINDOW_STATUS{
    BAD_GLAD_LOAD          = -4,
    BAD_GLFW_WINDOW_CREATE = -3,
    BAD_PTR                = -2,
    BAD_GLFW_INIT          = -1, 

    OK                     =  0,     
    WINDOW_STATUS_MAX      =  1
};

uint8_t win_init(const char* title, uint32_t height, uint32_t width, window* win);
void win_destroy(window* window);
void win_poll_events(window* win);
void win_swap_buffers(window* win);
bool win_should_close(window* win);

#endif 