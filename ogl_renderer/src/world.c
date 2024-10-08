#include "world.h"
#include "picel_math.h"
#include "event_system.h"
static struct store{
    ecs ecs;
    camera main_camera;
    shader basic_shader;
    mesh basic_mesh;
    window main_window;

    //mutex for each concurrent resources
} store;

void world_init(){
    //todo it is main event bus
    event_system_init();

    int8_t basic_shader_init_res;
    int8_t basic_mesh_init_res;

    //check
    store.ecs = ecs_init();


    if((store.main_window = win_init("asd", 1024, 2048)) == nullptr){
        exit(-1);
    }

    if((basic_shader_init_res = shader_init("./shaders/basic.vert", "./shaders/basic.frag", &store.basic_shader)) != SHADER_STATUS_OK){
        exit(basic_shader_init_res);
    }

    if((basic_mesh_init_res = mesh_init("TODO", &store.basic_mesh)) != MESH_STATUS_OK){
        exit(basic_mesh_init_res);
    }

        
    store.main_camera = camera_init(
        v3f(0, 5.0, 0),
        v3f(0, 0, -3.0f),
        v3f(0, 1.0, 0)
    );

    if(store.main_camera == nullptr){
        exit(-1);
    }
}

void world_destroy(){
    ecs_destroy(store.ecs);
    win_destroy(store.main_window);
    camera_destroy(store.main_camera);
    shader_destroy(&store.basic_shader);
    mesh_destroy(&store.basic_mesh);
}



// we need mutex and some amgic to protect this
ecs world_get_ecs(){
    return store.ecs;
}

camera world_get_main_camera(){
    return store.main_camera;
}

shader world_get_basic_shader(){
    return store.basic_shader;
}

mesh world_get_basic_mesh(){
    return store.basic_mesh;
}

window world_get_main_window(){
    return store.main_window;
}