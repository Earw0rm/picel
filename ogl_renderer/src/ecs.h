#ifndef _ECS_H_
#define _ECS_H_

#include "defines.h"
#include "entity_components.h"
#include "containers/darray.h"

typedef struct ecs_impl* ecs;

ecs ecs_init();
void ecs_destroy(ecs ecs);
ecs_entity ecs_create_entity(ecs ecs);
active_components_array ecs_has_components(ecs ecs, ecs_entity e);

/**
 * @param entity entity id 
 * @param component_id component_id
 * @return (void*) pointer to the component
 */
void* _ecs_get_component(ecs ecs, ecs_entity e, component_id id);
#define ecs_get_component(ecs, e, component_name) ((component_name*)_ecs_get_component(ecs, e, COMPONENT_ID_OF(component_name)))

void _ecs_add_components(ecs ecs, ecs_entity e, component_mask mask);
#define ecs_add_components(ecs, e, ...) _ecs_add_components(ecs, e, COMPONENT_MASK_OF_ALL(__VA_ARGS__))

void _ecs_remove_components(ecs ecs, ecs_entity e, component_mask mask);
#define ecs_remove_components(ecs, e, ...) _ecs_remove_components(ecs, e, COMPONENT_MASK_OF_ALL(__VA_ARGS__))

void ecs_destroy_entity(ecs ecs, ecs_entity e);

/**
 * This is private method. Use ecs_query(...);
 * @param component_count => num of components
 * @param ...             => component offsets inside ecs_component_set
 */
darray _ecs_query(ecs ecs, component_mask mask);
#define ecs_query(ecs, ...) _ecs_query(ecs, COMPONENT_MASK_OF_ALL(__VA_ARGS__))




#endif 