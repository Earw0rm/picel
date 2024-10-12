#ifndef _WINDOW_H_
#define _WINDOW_H_


#include <stdint.h>
#include <stdbool.h>
#include "math_struct.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
typedef struct window_impl* window;



window win_init(const char* title, uint32_t height, uint32_t width);

void win_destroy(window pwindow);
void win_poll_events(window pwindow);
void win_swap_buffers(window pwindow);
bool win_should_close(window pwindow);
float win_get_aspect_ratio(window pwindow);

matrix4f win_get_view(window pwindow);
GLFWwindow* win_get_glfw_ctx(window p);

#endif 