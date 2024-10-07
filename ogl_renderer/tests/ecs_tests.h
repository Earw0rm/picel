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

        ASSERT(ii == i, "ecs_init after destroy should behave correctly");

        ecs_add_components(ii, component_move, component_mesh);
        active_components_array aca = ecs_has_components(ii);
        ASSERT(aca == COMPONENT_MASK_OF_ALL(component_move, component_mesh), "after adding a component, the component mask should change");
    }

    for(int8_t i = 0; i < 10; ++i){
        ecs_destroy_entity(i);
        active_components_array aca = ecs_has_components(i);
        ASSERT((aca ==  COMPONENT_MASK_OF(unactive)), "after destroy a component, the component mask should change");
    }    

    ecs_destroy();
    return 1;
}

static inline int 
ecs_modify_component_data_test(){
    ecs_init();

    for(int8_t i = 0; i < 10; ++i){
        int8_t ii = ecs_create_entity();
        ecs_add_components(ii, component_move);
        component_move* cmove = ecs_get_component(ii, component_move);
        cmove->a = i * 5;
        cmove->aa = (char) ii;    
    }

    for(int8_t i = 0; i < 10; ++i){
        component_move* cmove = ecs_get_component(i, component_move);
        ASSERT(cmove->a == i*5, "data should be correctly stored inside ecs component");
        ASSERT(cmove->aa == (char)i, "data should be correctly stored inside ecs component");

        active_components_array aca = ecs_has_components(i);
        ASSERT((aca & COMPONENT_MASK_OF(component_move)) == COMPONENT_MASK_OF(component_move), "components should have valid mask");
        
        ecs_remove_components(i, component_move);
        aca = ecs_has_components(i);
        ASSERT((aca & COMPONENT_MASK_OF(component_move)) != COMPONENT_MASK_OF(component_move), "components should have valid mask after remove compnent");
    }

    for(int8_t i = 0; i < 10; ++i){
        component_move* cmove = ecs_get_component(i, component_move);
        ASSERT(cmove->a == i*5, "data should be correctly stored inside ecs component");
        ASSERT(cmove->aa == (char)i, "data should be correctly stored inside ecs component");
    }


    ecs_destroy();
    return 1;
}

static inline void 
ecs_run_tests(){
    TEST("ECS Entity creation and destroy basic", ecs_basic_test);
    TEST("ECS basic operations with components", ecs_basic_component_set);
    TEST("ECS modify component data test", ecs_modify_component_data_test);
}
#endif 