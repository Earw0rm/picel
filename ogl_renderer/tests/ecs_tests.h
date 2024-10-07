#ifndef _ECS_TESTS_H_
#define _ECS_TESTS_H_
#include "yasyc_test.h"
#include "ecs.h"


static inline int 
ecs_basic_test(){


    ecs_init();

    for(int8_t i = 0; i < 10; ++i){
        int8_t ii = ecs_create_entity();
        ASSERT(i == ii, "Entity id in ecs must be sequentialy, starting from zero");
    }

    for(int8_t i = 0; i < 5; ++i){
        ecs_destroy_entity(i);
    }

    for(int8_t i = 0; i < 5; ++i){
        int8_t ii = ecs_create_entity();
        ASSERT(i == ii, "entities must be restored from the queue after deletion");
    }

    ecs_destroy();
    return 1;
}


static inline int 
ecs_basic_component_set(){

    ecs_init();
    for(int8_t i = 0; i < 10; ++i){
        int8_t ii = ecs_create_entity();

        ecs_add_components(ii, component_move, component_mesh);
        component_mask mask = ecs_has_components(ii, component_move, component_mesh);
        ASSERT(mask == COMPONENT_MASK_OF_ALL(component_move, component_mesh), "after adding a component, the component mask should change");
    }


    ecs_destroy();
    return 1;
}

static inline void 
ecs_run_tests(){
    TEST("ECS Entity creation and destroy basic", ecs_basic_test);
    TEST("ECS basic operations with components", ecs_basic_component_set);
}
#endif 