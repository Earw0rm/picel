#ifndef _ECS_TESTS_H_
#define _ECS_TESTS_H_
#include "yasyc_test.h"
#include "ecs.h"
#include "picel_math.h"

static inline int 
ecs_basic_test(){


    ecs ecs = ecs_init();

    for(int8_t i = 0; i < 10; ++i){
        int8_t ii = ecs_create_entity(ecs);
        ASSERT(i == ii, "Entity id in ecs must be sequentialy, starting from zero");
    }

    for(int8_t i = 0; i < 5; ++i){
        ecs_destroy_entity(ecs, i);
    }

    for(int8_t i = 0; i < 5; ++i){
        int8_t ii = ecs_create_entity(ecs);
        ASSERT(i == ii, "entities must be restored from the queue after deletion");
    }

    ecs_destroy(ecs);
    return 1;
}


static inline int 
ecs_basic_component_set(){

    ecs ecs = ecs_init();
    for(int8_t i = 0; i < 10; ++i){
        int8_t ii = ecs_create_entity(ecs);

        ASSERT(ii == i, "ecs_init after destroy should behave correctly");

        ecs_add_components(ecs, ii, component_transform, component_mesh);
        active_components_array aca = ecs_has_components(ecs, ii);

        ASSERT(aca == COMPONENT_MASK_OF_ALL(component_transform, component_mesh), "after adding a component, the component mask should change");
    }

    for(int8_t i = 0; i < 10; ++i){
        ecs_destroy_entity(ecs, i);
        active_components_array aca = ecs_has_components(ecs, i);
        ASSERT((aca ==  COMPONENT_MASK_OF(unactive)), "after destroy a component, the component mask should change");
    }    

    ecs_destroy(ecs);
    return 1;
}

static inline int 
ecs_modify_component_data_test(){
    ecs ecs = ecs_init();

    for(int8_t i = 0; i < 10; ++i){
        int8_t ii = ecs_create_entity(ecs);
        ecs_add_components(ecs, ii, component_transform);
        component_transform* cmove = ecs_get_component(ecs, ii, component_transform);
        
        cmove->position = v3f(i*5, 1, 1);
        cmove->rotation = v3f(1, 1, i*10);    
    }

    for(int8_t i = 0; i < 10; ++i){
        component_transform* cmove = ecs_get_component(ecs, i, component_transform);
        ASSERT(cmove->position.x == i*5, "data should be correctly stored inside ecs component");
        ASSERT(cmove->rotation.z == i*10, "data should be correctly stored inside ecs component");

        active_components_array aca = ecs_has_components(ecs, i);
        ASSERT((aca & COMPONENT_MASK_OF(component_transform)) == COMPONENT_MASK_OF(component_transform), "components should have valid mask");
        
        ecs_remove_components(ecs, i, component_transform);
        aca = ecs_has_components(ecs, i);
        ASSERT((aca & COMPONENT_MASK_OF(component_transform)) != COMPONENT_MASK_OF(component_transform), "components should have valid mask after remove compnent");
    }

    for(int8_t i = 0; i < 10; ++i){
        component_transform* cmove = ecs_get_component(ecs, i, component_transform);
        ASSERT(cmove->position.x == i*5, "data should be correctly stored inside ecs component");
        ASSERT(cmove->rotation.z == i*10, "data should be correctly stored inside ecs component");
    }


    ecs_destroy(ecs);
    return 1;
}

static inline void 
ecs_run_tests(){
    TEST("ECS Entity creation and destroy basic", ecs_basic_test);
    TEST("ECS basic operations with components", ecs_basic_component_set);
    TEST("ECS modify component data test", ecs_modify_component_data_test);
}
#endif 