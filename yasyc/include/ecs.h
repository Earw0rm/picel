#ifndef _ECS_H_
#define _ECS_H_

#include "defines.h"

#define ecs_init(...) _ecs_init(VA_NARGS(__VA_ARGS__), SIZEOF_ALL(__VA_ARGS__))
#define ecs_query(...) _ecs_query(VA_NARGS(__VA_ARGS__), __VA_ARGS__)

typedef uint64_t ecs_entity;

typedef struct ecs_query_result{
    uint64_t count;
    uint64_t cap;
    uint64_t *lst;
} ecs_query_result;

/**
 * @param component_count number of components that ecs store. 
 * @param ... sizeof(ComponentType)
 */
API void  _ecs_init(uint64_t component_count, ...);
API void   ecs_destroy();
API ecs_entity ecs_create_entity();
API void*  ecs_get_component(ecs_entity e, uint64_t component_id);
API void   ecs_add_component(ecs_entity e, uint64_t component_id, void* component);
API void   ecs_remove_component(ecs_entity e, uint64_t component_id);
API void   ecs_has_component(ecs_entity e, uint64_t component_id);
API void   ecs_destroy_entity(ecs_entity e);

API ecs_query_result* _ecs_query(uint64_t component_count, ...);


#endif 