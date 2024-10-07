#ifndef _ECS_H_
#define _ECS_H_

#include "defines.h"
#include "entity_components.h"
#include "containers/darray.h"


void ecs_init();
void ecs_destroy();
ecs_entity ecs_create_entity();
active_components_array ecs_has_components(ecs_entity e);

/**
 * @param entity entity id 
 * @param component_id component_id
 * @return (void*) pointer to the component
 */
void* _ecs_get_component(ecs_entity e, component_id id);
#define ecs_get_component(e, component_name) ((component_name*)_ecs_get_component(e, COMPONENT_ID_OF(component_name)))

void _ecs_add_components(ecs_entity e, component_mask mask);
#define ecs_add_components(e, ...) _ecs_add_components(e, COMPONENT_MASK_OF_ALL(__VA_ARGS__))

void _ecs_remove_components(ecs_entity e, component_mask mask);
#define ecs_remove_components(e, ...) _ecs_remove_components(e, COMPONENT_MASK_OF_ALL(__VA_ARGS__))

void ecs_destroy_entity(ecs_entity e);

/**
 * This is private method. Use ecs_query(...);
 * @param component_count => num of components
 * @param ...             => component offsets inside ecs_component_set
 */
darray _ecs_query(component_mask mask);
#define ecs_query(...) _ecs_query(COMPONENT_MASK_OF_ALL(__VA_ARGS__))




#endif 