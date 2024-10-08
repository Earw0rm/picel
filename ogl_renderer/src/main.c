



#include "window.h"
#include "picel_math.h"
#include "event_system.h"
#include "ecs.h"
#include "shader.h"
#include "uvn_camera.h"
#include "mesh.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include "world.h"
#include <cglm/cglm.h>   /* for inline */
// #include <cglm/call.h>   /* for library call (this also includes cglm.h) */

// TODO this is resources
// now we only support one basic shader programm (fragment and vertex) and only one main camera
// TODO windows is resources too
static shader basic_shader;
static camera main_camera;
static mesh basic_cube_mesh;
//static window window;


static void 
basic_resources_initialization(){
        uint8_t win_init_res;
        int8_t basic_shader_init_res;
        int8_t basic_mesh_init_res;

        ecs_init();    
        event_system_init();

        if((win_init_res = win_init("asd", 1024, 2048)) != SHADER_STATUS_OK){
            exit(win_init_res);
        }

        if((basic_shader_init_res = shader_init("./shaders/basic.vert", "./shaders/basic.frag", &basic_shader)) != SHADER_STATUS_OK){
            exit(basic_shader_init_res);
        }

        if((basic_mesh_init_res = mesh_init("TODO", &basic_cube_mesh)) != MESH_STATUS_OK){
            exit(basic_mesh_init_res);
        }

        // TODO check
        main_camera = camera_init(
            v3f(0, 0, 5.0f),
            v3f(0, 0, -3.0),
            v3f(0, 1.0, 0)
        );




}


// TODO
// this function is resource manager->create_basic_cube;
static void 
basic_scene_setup(){
    ecs ecs = world_get_ecs();
    ecs_entity cube_entity = ecs_create_entity(ecs);
    component_material* cmat = ecs_get_component(ecs, cube_entity, component_material);
    component_transform* ctrans = ecs_get_component(ecs,cube_entity, component_transform);
    component_mesh* cmesh = ecs_get_component(ecs,cube_entity, component_mesh);


    //basic material for our cube
    cmat->shader_program = basic_shader.program;
    cmat->texture_count  = 0;
    cmat->textures = nullptr;

    //starting basic position for our cube. Basicli it is just model matrix
    ctrans->position = v3f(0, 0, 0);
    ctrans->scale    = v3f(0, 0, 0);
    ctrans->rotation = v3f(0, 0, 0);

    // basic mesh component
    cmesh->ebo = basic_cube_mesh.ebo;
    cmesh->vbo = basic_cube_mesh.vbo;
    cmesh->vao = basic_cube_mesh.vao;
    cmesh->vectex_count = basic_cube_mesh.vertices_num; // vertex_count?


    ecs_add_components(ecs, cube_entity, component_material, component_transform, component_mesh, component_renderable);
}

int main(int argc, char const *argv[]){
        world_init();
        basic_resources_initialization();
        basic_scene_setup();





        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        float aspect_ratio = win_get_aspect_ratio();

        while(!win_should_close()){


            // const float radius = 2.0f;
            // float camX = sin(glfwGetTime()) * radius;
            // float camZ = cos(glfwGetTime()) * radius;
            // camera_set_position(main_camera, camX, 0.0, camZ);

            matrix4f view = camera_get_view(main_camera);
           

            matrix4f mvp = mat4f_id(1);
            mvp = mat4f_scale(mvp, 0.5f, 0.5f, 0.5f);
            mvp = mat4f_rotate(mvp, 0.0, 0.0, 0.0);
            mvp = mat4f_translate(mvp, 0.0, 0.0f, -3.0f);      

            mvp = mdotm4(view, mvp);

            mvp = mat4f_project(mvp, 20.0f, aspect_ratio, 1, 100);

            // mat4 dst;
            // glm_perspective(glm_rad(40), aspect_ratio, 1, 100, dst);
            
            // matrix4f tmp = mat4f_from_cglm(dst);
            // mvp = mdotm4(tmp, mvp);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // glUniformMatrix4fv(basic_shader.mvp_loc, 1, GL_FALSE, mvp.m);
            glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, 0 /*last parameter deprecated*/);


            win_swap_buffers();
            win_poll_events();
            event_system_process_all();

        }
        glBindVertexArray(0);

        camera_destroy(main_camera);
        event_system_destroy();
        exit(EXIT_SUCCESS);
}
