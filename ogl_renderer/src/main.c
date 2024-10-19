#include "window.h"
#include "picel_math.h"
#include "event_system.h"
#include "ecs.h"
#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "render_system.h"
#include "texture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include "world.h"
#include "gui_system.h"
// this 2 include for library testing if we need to compare result against hand made picel_math.h
// #include <cglm/cglm.h>   /* for inline */
// #include <cglm/call.h>   /* for library call (this also includes cglm.h) */

static void 
basic_scene_setup(){
    ecs ecs = world_get_ecs();
    shader basic_shader = world_get_basic_shader();
    mesh basic_cube_mesh = world_get_basic_mesh();
    texture basic_texture = world_get_basic_texture();


    ecs_entity cube_entity = ecs_create_entity(ecs);
    component_material* cmat = ecs_get_component(ecs, cube_entity, component_material);
    component_transform* ctrans = ecs_get_component(ecs,cube_entity, component_transform);
    component_mesh* cmesh = ecs_get_component(ecs,cube_entity, component_mesh);


    //basic material for our cube
    cmat->shader_program = basic_shader.program;
    cmat->texture_obj  = basic_texture.texture_obj;

    //starting basic position for our cube. Basicli it is just model matrix
    ctrans->position = v3f(0, 0, 0);
    ctrans->scale    = v3f(1, 1, 1);
    ctrans->rotation = v3f(0, 0, 0);

    // basic mesh component
    cmesh->ebo = basic_cube_mesh.ebo;
    cmesh->vbo = basic_cube_mesh.vbo;
    cmesh->vao = basic_cube_mesh.vao;
    cmesh->vectex_count = basic_cube_mesh.vertices_num; // vertex_count?


    ecs_add_components(ecs, cube_entity, component_material, component_transform, component_mesh, component_renderable);


    //second object (light source)
    basic_shader = world_get_basic_shader();
    basic_cube_mesh = world_get_basic_mesh();
    basic_texture = world_get_basic_texture();


    ecs_entity light_cube_entity = ecs_create_entity(ecs);
    cmat = ecs_get_component(ecs, light_cube_entity, component_material);
    ctrans = ecs_get_component(ecs, light_cube_entity, component_transform);
    cmesh = ecs_get_component(ecs, light_cube_entity, component_mesh);


    //basic material for our cube
    cmat->shader_program = basic_shader.program;
    cmat->texture_obj  = 0;

    //starting basic position for our cube. Basicli it is just model matrix
    ctrans->position = v3f(-2, 2, -5);
    ctrans->scale    = v3f(1, 1, 1);
    ctrans->rotation = v3f(0, 0, 0);

    // basic mesh component
    cmesh->ebo = basic_cube_mesh.ebo;
    cmesh->vbo = basic_cube_mesh.vbo;
    cmesh->vao = basic_cube_mesh.vao;
    cmesh->vectex_count = basic_cube_mesh.vertices_num; // vertex_count?

    ecs_add_components(ecs, light_cube_entity, component_material, component_transform, component_mesh, component_renderable);

}

int main(int argc, char const *argv[]){
        world_init();
        //should be in world? no
        //todo now it is crunch
        basic_scene_setup();

        ecs ecs = world_get_ecs();
        camera main_camera = world_get_main_camera();
        window main_window = world_get_main_window();

        gui_system_init(main_window, 512 * 1024, 128 * 1024);


        while(!win_should_close(main_window)){

            render_system_render(ecs, main_window, main_camera);
            gui_system_render(main_window);

            win_swap_buffers(main_window);
            win_poll_events(main_window);
            event_system_process_all();

        }

        gui_system_destroy();
        world_destroy();
        exit(EXIT_SUCCESS);
}
