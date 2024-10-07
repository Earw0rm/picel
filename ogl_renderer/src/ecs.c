#include "ecs.h"
#include "containers/dqueue.h"
#include "logger.h"





/**
 * Each entity is a pair of <id, bitmask>.
 * If bitmask < 0, then entity is inactive, and  should be stored in inactive_entities dqueue.
 * Otherwise each bit inside bitbask represent that entity has nth component.
 * 
 * Components darray store "line" or "stream" of bits.
 * 
 * Each line is a dense packed set of components that may or may not be present in the entity, 
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
    dqueue inactive_entities;
    darray components;
    darray query_res;
} store;





void ecs_init(){
    store.inactive_entities = dqueue_alloc(int64_t);
    store.components = darray_alloc(ecs_component_set);
    store.query_res = darray_alloc(ecs_entity);

}
void ecs_destroy(){
    dqueue_free(store.inactive_entities);
    darray_free(store.components);    
    darray_free(store.query_res);        
    store.entity_allocator = 0;
}

ecs_entity ecs_create_entity(){
    ecs_entity* pres;
    if((pres = dqueue_pop_front(store.inactive_entities)) != nullptr){
        return *pres;
    }

    ecs_entity res = store.entity_allocator++; 
    ecs_component_set zero_set = {0};
    darray_push_back(store.components, zero_set);

    return res;
}



/**
 * @param entity entity id 
 * @param component_id component_id
 * @return (void*) pointer to the component
 */
void* _ecs_get_component(ecs_entity e, component_id id){
    ecs_component_set* cs = (ecs_component_set* ) darray_at(store.components, e);
    return (void*)((char*)cs + id);
}

active_components_array ecs_has_components(ecs_entity e){
    ecs_component_set* cs = (ecs_component_set* ) darray_at(store.components, e);

    return cs->arr;
}

void _ecs_remove_components(ecs_entity e, component_mask mask){
    ecs_component_set* cs = (ecs_component_set* ) darray_at(store.components, e);
    cs->arr = (cs->arr &(~mask));
}

void _ecs_add_components(ecs_entity e, component_mask mask){
    ecs_component_set* cs = (ecs_component_set* ) darray_at(store.components, e);
    cs->arr = (cs->arr | mask);

}

void ecs_destroy_entity(ecs_entity e){
    ecs_component_set* cs = (ecs_component_set* ) darray_at(store.components, e);
    cs->arr = COMPONENT_MASK_OF(unactive); 
    dqueue_push_back(store.inactive_entities, e);
}

// It is expected that the mask will contain that the component is active if need
darray _ecs_query(component_mask mask){
    darray_clear(store.query_res);
    for(uint64_t i = 0; i < darray_lenght(store.components); ++i){
        ecs_component_set* cs = (ecs_component_set* ) darray_at(store.components, i);
        if((cs->arr & mask) == mask){
            darray_push_back(store.query_res, i);
        }
    }
    return store.query_res;
}