#include "window.h"
#include "picel_math.h"
#include "event_system.h"
#include "ecs.h"
#include "shader.h"
#include "uvn_camera.h"
#include "mesh.h"
#include "render_system.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include "world.h"
#include <cglm/cglm.h>   /* for inline */
// #include <cglm/call.h>   /* for library call (this also includes cglm.h) */

static void 
basic_scene_setup(){
    ecs ecs = world_get_ecs();
    shader basic_shader = world_get_basic_shader();
    mesh basic_cube_mesh = world_get_basic_mesh();

    ecs_entity cube_entity = ecs_create_entity(ecs);
    component_material* cmat = ecs_get_component(ecs, cube_entity, component_material);
    component_transform* ctrans = ecs_get_component(ecs,cube_entity, component_transform);
    component_mesh* cmesh = ecs_get_component(ecs,cube_entity, component_mesh);


    //basic material for our cube
    cmat->shader_program = basic_shader.program;
    cmat->texture_count  = 0;
    cmat->textures = nullptr;

    //starting basic position for our cube. Basicli it is just model matrix
    ctrans->position = v3f(0, 0, -3.0f);
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
        //should be in world? no
        //todo now it is crunch
        basic_scene_setup();


        ecs ecs = world_get_ecs();
        camera main_camera = world_get_main_camera();
        window main_window = world_get_main_window();

        while(!win_should_close(main_window)){

            render_system_render(ecs, main_window, main_camera);
            win_swap_buffers(main_window);
            win_poll_events(main_window);
            event_system_process_all();

        }

        world_destroy();
        exit(EXIT_SUCCESS);
}
