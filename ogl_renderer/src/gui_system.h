#ifndef _GUI_SYSTEM_H_
#define _GUI_SYSTEM_H_
#include "window.h"



void gui_system_init(window win, uint32_t max_vertex_buffer, uint32_t max_element_buffer);

void gui_system_render(window win);

void gui_system_destroy();


#endif 