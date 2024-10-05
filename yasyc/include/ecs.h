#ifndef _ECS_H_
#define _ECS_H_

#include "defines.h"

#define ecs_init(...) _ecs_init(VA_NARGS(__VA_ARGS__), SIZEOF_ALL(__VA_ARGS__))
#define ecs_query(...) _ecs_query(VA_NARGS(__VA_ARGS__), __VA_ARGS__)

typedef uint64_t entity;

typedef struct ecs_query_result{
    uint64_t count;
    uint64_t cap;
    uint64_t *lst;
} ecs_query_result;
/**
 * @param component_count number of components that ecs store. 
 * @param ... sizeof(ComponentType)
 */
void  _ecs_init(uint64_t component_count, ...);
entity ecs_create_entity();
void* ecs_get_component(entity e, uint64_t component_id);
void  ecs_add_component(entity e, uint64_t component_id, void* component);
void  ecs_remove_component(entity e, uint64_t component_id);
void  ecs_has_component(entity e, uint64_t component_id);
void  ecs_destroy_entity(entity e);

ecs_query_result* _ecs_query(uint64_t component_count, ...);


#endif 