#include "logger.h"
#include "window.h"

#include "shader.h"
#include <stdio.h>



int main(int argc, char const *argv[])
{
    if(win_init("my first c window", 1024, 1024)){
        return -1;
    }


    int8_t shader_init_res;
    shader basic_shader;
    if((shader_init_res = shader_init("./shaders/basic.vert", "./shaders/basic.frag", &basic_shader)) != 0){
        return shader_init_res;
    }
    shader_use(&basic_shader);


        GLuint vbo, vao, ebo;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);



        float cube[8 * 7] = {
            /*vertice coordinat*/              /*vertex color*/ 
            /*0*/-1.0f, 1.0f,-1.0f,            1.0f, 0.0f, 1.0f, 1.0f,
            /*1*/ 1.0f, 1.0f,-1.0f,            0.5f, 0.5f, 0.0f, 1.0f,
            /*2*/ 1.0f, 1.0f, 1.0f,            0.0f, 1.0f, 0.0f, 1.0f,
            /*3*/-1.0f, 1.0f, 1.0f,            0.0f, 0.5f, 0.5f, 1.0f,
            /*4*/-1.0f,-1.0f,-1.0f,            0.0f, 0.0f, 0.1f, 1.0f,
            /*5*/ 1.0f,-1.0f,-1.0f,            0.5f, 0.0f, 0.5f, 1.0f,
            /*6*/ 1.0f,-1.0f, 1.0f,            0.5f, 0.5f, 0.5f, 1.0f,
            /*7*/-1.0f,-1.0f, 1.0f,            1.0f, 1.0f, 1.0f, 1.0f,                                                                                    
        };
        [[gnu::unused]]GLuint indices[12 * 3] ={
            0, 1, 3, 
            1, 2, 3,
            0, 4, 1,
            1, 4, 5,
            0, 3, 7,
            0, 7, 4,
            1, 6, 2,
            1, 5, 6,
            2, 7, 3,
            2, 6, 7,
            4, 7, 5,
            5, 7, 6                                                                                                             
        };


        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);    
        // load data from memory to GPU
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);    
        // load data from memory to GPU
        glBufferData(GL_ARRAY_BUFFER, 8 * 7* sizeof(float), cube, GL_STATIC_DRAW);


        
        /*next 3 calls describe how to use our VBO*/
        // glVertexAttribPointer call work relative last binded buffer
        glVertexAttribPointer( /*lenght = 3 (3 vertises). Stride => vertices + colors*/
            /*layout (location = 0)*/ 0,
            /*how many data for vertices*/3,
            GL_FLOAT,
            GL_FALSE,
            7 * sizeof(float), // stride
            /*offset inside VBO*/(void*)0
        );
        glVertexAttribPointer( 
            /*layout (location = 0)*/ 1,
            /*how many data for colors*/4,
            GL_FLOAT,
            GL_FALSE,
            7 * sizeof(float),
            /*offset inside VBO*/(void*)((3 * sizeof(float)))
        );

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);



    while(!win_should_close()){
        glClear(GL_COLOR_BUFFER_BIT );
        glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, 0);

        win_swap_buffers();
        win_poll_events();
    }

    win_destroy();
    return 0;
}
