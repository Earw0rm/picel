#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <stdint.h>
#include <stdbool.h>



enum WINDOW_STATUS{
    BAD_GLAD_LOAD          = -4,
    BAD_GLFW_WINDOW_CREATE = -3,
    BAD_PTR                = -2,
    BAD_GLFW_INIT          = -1, 

    OK                     =  0,     
    WINDOW_STATUS_MAX      =  1
};

uint8_t win_init(const char* title, uint32_t height, uint32_t width);
void win_destroy();
void win_poll_events();
void win_swap_buffers();
bool win_should_close();

#endif 