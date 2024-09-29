#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "window.h"
#include "math.h"

#include "shader.h"
#include <stdlib.h>

int main(int argc, char const *argv[])
{       uint8_t win_init_res;
        if((win_init_res = win_init("asd", 1024, 1024)) != SHADER_STATUS_OK){
            return win_init_res;
        }

        int8_t shader_res;
        shader basic_shader;
        if((shader_res = shader_init("./shaders/basic.vert", "./shaders/basic.frag", &basic_shader)) != SHADER_STATUS_OK){
            exit(shader_res);
 
        }

        if((shader_res = shader_use(&basic_shader)) != SHADER_STATUS_OK){
            exit(shader_res);
        }





        GLuint vbo, vao, ebo;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);


        float cube[3 * 7] = {
            /*vertice coordinat*/              /*vertex color*/ 
            /*0*/ 0.0f, 0.0f, 0.0f,            1.0f, 0.0f, 0.0f, 1.0f,
            /*1*/ 0.5f, -0.5f, 1.0f,           0.0f, 1.0f, 0.0f, 1.0f,
            /*2*/-0.5f,-0.5f, 0.0f,            0.0f, 0.0f, 1.0f, 1.0f                                              
        };
        [[gnu::unused]]GLuint indices[1 * 3] ={
            0, 1, 2                                                                                                           
        };


        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);    
        // load data from memory to GPU
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 1 * 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);    
        // load data from memory to GPU
        glBufferData(GL_ARRAY_BUFFER, 3 * 7* sizeof(float), cube, GL_STATIC_DRAW);


        
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

            matrix4f mvp = mat4f_id(1);
            mvp = mat4f_scale(mvp, 2, 2, 0);



            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUniformMatrix4fv(basic_shader.mvp_loc, 1, GL_FALSE, mvp.m);
            // for data withou ebo
            // glDrawArrays(GL_TRIANGLES, 0, vert_count);
            // for data with ebo
            glDrawElements(GL_TRIANGLES, 1 * 3, GL_UNSIGNED_INT, 0 /*last parameter deprecated*/);

            win_swap_buffers();
            win_poll_events();
        }
        glBindVertexArray(0);
        exit(EXIT_SUCCESS);
}
