#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdint.h>
#include <stdbool.h>
#include "math_struct.h"


enum WINDOW_STATUS{
    BAD_DOUBLE_INITIALIZATION = -4,
    BAD_GLAD_LOAD             = -3,
    BAD_GLFW_WINDOW_CREATE    = -2,
    BAD_GLFW_INIT             = -1, 

    OK                        =  0,     
    WINDOW_STATUS_MAX         =  1
};


uint8_t win_init(const char* title, uint32_t height, uint32_t width);

void win_destroy();
void win_poll_events();
void win_swap_buffers();
bool win_should_close();
float win_get_aspect_ratio();

//TODO CRUNCH
matrix4f win_get_view();
#endif 