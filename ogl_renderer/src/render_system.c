// #include "render_system.h"
// #include "containers/darray.h"
// #include "ecs.h"
// #include "world.h"

// [[gnu::unused]]void render_system_render(){
//     ecs ecs = world_get_ecs();
//     [[gnu::unused]]darray qres = ecs_query(ecs, component_transform, component_mesh, component_material, component_renderable);
//     for(uint64_t i = 0; i < darray_lenght(qres); ++i){
//         [[gnu::unused]]component_transform* ctrans = ecs_get_component(ecs, i, component_transform);
//         [[gnu::unused]]component_mesh* cmesh = ecs_get_component(ecs, i, component_mesh);
//         [[gnu::unused]]component_material* cmat = ecs_get_component(ecs, i, component_material);
        
        
//         // component_transform* ct = ecs_get_component(i, component_renderable);
//     } 
// }