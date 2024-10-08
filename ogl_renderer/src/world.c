#include "world.h"


static struct store{
    ecs ecs;
    camera main_camera;
    shader basic_shader;
    mesh basic_mesh;


    //mutex for each concurrent resources
};

void world_init(){
    // init all global resources resources
}

void world_destroy(){

}

ecs world_get_ecs(){
    return nullptr;
}

camera world_get_main_camera(){
    return nullptr;
}

shader world_get_basic_shader(){
    return nullptr;
}
mesh world_get_basic_mesh(){
    return nullptr;
}
