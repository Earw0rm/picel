#include "logger.h"
#include "window.h"
#include "vector.h"
#include "shader.h"
#include <stdio.h>

static GLuint VBO;

static void render_scene_cb(){
    glClear(GL_COLOR_BUFFER_BIT);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_POINT, 0, 1);
    glDisableVertexAttribArray(0);


    win_swap_buffers();
    win_poll_events();
}

static void create_vertex_buffer(){
    vector3f vertices[1] = {
        v3f(0.0f)
    };

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}


int main(int argc, char const *argv[])
{
    if(win_init("my first c window", 2048, 2048)){
        return -1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    create_vertex_buffer();

    int8_t shader_init_res;
    shader basic_shader;

    if((shader_init_res = shader_init("./shaders/basic.vert", "./shaders/basic.frag", &basic_shader)) != 0){
        return shader_init_res;
    }
    LOG_INFO("shader is initialized.Program: %i, Vertex_shader: %i, Fragment_shader: %i",
              basic_shader.program,
              basic_shader.vert_shader_id, 
              basic_shader.frag_shader_id);

    while(!win_should_close()){
        render_scene_cb();
    }

    win_destroy();
    return 0;
}
