#include "ecs.h"
#include "entity.h"
#include "containers/darray.h"
#include "containers/dqueue.h"
#include <varargs.h>

/**
 * 
 * Each entity is a pair of <id, bitmask>.
 * If bitmask < 0, then entity is inactive, and  should be stored in inactive_entities dqueue.
 * Otherwise each bit inside bitbask represent that entity has nth component.
 * 
 * Components darray store "line" or "stream" of bits.
 * 
 * Each line is a densely packed set of components that may or may not be present in the entity, 
 * plus int64_t that represent bitmask. The ith element in the array represents an entity with id = i.
 * The more possible components an application has, the larger the line will be.
 * 
 * For example, if out application has 3 components, 12, 14 and 18 bytes long, then our line will be
 * (12 + 14 + 18) + 8(bitmask) bytes respectivle.  
 * 
 * It is important to note that ecs itself does not perform alignment. 
 * It is expected that alignment will be at the level of component structures that are registered 
 * in the "store" by calling ecs_init.
 */
static struct {
    uint64_t entity_allocator;
    dqueue   inactive_entities;
    darray   components;
} store;

static ecs_query_result query_result;


void _ecs_init(uint64_t component_count, ...){
    va_list ap;
    uint32_t total_line_sz = 8;

    va_start(ap, num);

    // There can't be more than 63 components now 
    for(int8_t i = 0; i < component_count; ++i){
        total_line_sz += va_arg(ap, uint32_t);
    }

    va_end(ap);

    store.inactive_entities = dqueue_alloc(int64_t);
    store.components = darray_alloc_fix(total_line_sz);
}

void ecs_destroy(){
    dqueue_free(store.inactive_entities);
    darray_free(store.components);    
}

API entity ecs_create_entity(){
    ecs_entity* pres;
    if((pres = dqueue_pop_front(store.inactive_entities) != nullptr){
        return *pres;
    }

    ecs_entity res;
    uint64_t components_capacity;
    
    if((res = ++store.entity_allocator) >= 
                            (components_capacity = darray_capacity(store.components))){
        darray_reserve(da, components_capacity * DARRAY_SCALE_FACTOR);
    }
    return res;
}

API void*  ecs_get_component(ecs_entity e, uint64_t component_id);
API void   ecs_add_component(ecs_entity e, uint64_t component_id, void* component);
API void   ecs_remove_component(ecs_entity e, uint64_t component_id);
API void   ecs_has_component(ecs_entity e, uint64_t component_id);
API void   ecs_destroy_entity(ecs_entity e);

API ecs_query_result* _ecs_query(uint64_t component_count, ...);