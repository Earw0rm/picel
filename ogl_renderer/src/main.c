



#include "window.h"
#include "math.h"
#include "event_system.h"

#include "shader.h"
#include "uvn_camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include <cglm/cglm.h>   /* for inline */
// #include <cglm/call.h>   /* for library call (this also includes cglm.h) */

int main(int argc, char const *argv[]){
    
        event_system_init();
        // TODO check
        camera main_camera = camera_init(
            v3f(0, 0, 5.0f),
            v3f(0, 0, -3.0),
            v3f(0, 1.0, 0)
        );

        uint8_t win_init_res;
        if((win_init_res = win_init("asd", 1024, 2048)) != SHADER_STATUS_OK){
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

        float cube[8 * 7] = {
             0.5f,  0.5f,  0.5f,        1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,        0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,        0.0f, 0.0f, 1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,        0.5f, 0.5f, 0.5f, 1.0f,

            -0.5f, -0.5f, -0.5f,        0.8f, 0.0f, 0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,        0.0f, 0.8f, 0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,        0.0f, 0.0f, 0.8f, 1.0f,
            -0.5f, -0.5f,  0.5f,        0.2f, 0.2f, 0.2f, 1.0f
        };


        [[gnu::unused]]GLuint indices[12 * 3] ={
            0, 1, 2,
            1, 3, 4,
            5, 6, 3,
            7, 3, 6,
            2, 4, 7,
            0, 7, 6,
            0, 5, 1,
            1, 5, 3,
            5, 0, 6, 
            7, 4, 3,
            2, 1, 4,
            0, 2, 7 
        };


        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);    
        // load data from memory to GPU
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);    
        // load data from memory to GPU
        glBufferData(GL_ARRAY_BUFFER, 8 * 7 * sizeof(float), cube, GL_STATIC_DRAW);


        
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
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);




        [[gnu::unused]]float aspect_ratio = win_get_aspect_ratio();
        [[gnu::unused]]float degree = 0.0f;
        while(!win_should_close()){


            // const float radius = 2.0f;
            // float camX = sin(glfwGetTime()) * radius;
            // float camZ = cos(glfwGetTime()) * radius;
            // camera_set_position(main_camera, camX, 0.0, camZ);

            matrix4f view = camera_get_view(main_camera);
           

            matrix4f mvp = mat4f_id(1);
            mvp = mat4f_scale(mvp, 0.5f, 0.5f, 0.5f);
            mvp = mat4f_rotate(mvp, 0.0, 0.0, degree);
            mvp = mat4f_translate(mvp, 0.0, 0.0f, -3.0f);      

            mvp = mdotm4(view, mvp);

            mvp = mat4f_project(mvp, 40.0f, aspect_ratio, 1, 100);

            // mat4 dst;
            // glm_perspective(glm_rad(40), aspect_ratio, 1, 100, dst);
            
            // matrix4f tmp = mat4f_from_cglm(dst);
            // mvp = mdotm4(tmp, mvp);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUniformMatrix4fv(basic_shader.mvp_loc, 1, GL_FALSE, mvp.m);
            glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, 0 /*last parameter deprecated*/);



            win_swap_buffers();
            win_poll_events();
            event_system_process_all();
            degree += 1.0f;
        }
        glBindVertexArray(0);

        camera_destroy(main_camera);
        event_system_destroy();
        exit(EXIT_SUCCESS);
}
