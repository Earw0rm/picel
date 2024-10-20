#ifndef _COMPONENTS_H_
#define _COMPONENTS_H_

#include "math_struct.h"
#include <glad/glad.h>

#include <stdint.h>
#include <stddef.h>

#define LIST_OF_COMPONENTS \
    X(component_transform) \
    X(component_mesh) \
    X(component_material) \
    X(component_renderable) \

typedef uint64_t ecs_entity;
typedef int64_t active_components_array;
//component id it is offset inside ecs_component_set
typedef uint64_t component_id;


typedef struct component_transform{
    vector3f position;
    vector3f rotation;
    vector3f scale;    
} component_transform;

typedef struct component_mesh{
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    size_t vectex_count;
}component_mesh;




typedef struct component_material{
    GLuint shader_program;
    GLuint texture_obj; 

    vector3f ambient;
    vector3f diffuse;
    vector3f specular;  
    float shininnes;
} component_material;

//component marker
typedef struct component_renderable{

}component_renderable;



typedef enum {
    
    #define X(arg) COMPONENT_MASK_ ## arg,
    LIST_OF_COMPONENTS
    #undef X

    COMPONENT_MASK_unactive = 63,
    COMPONENT_MASK_MAX = 64
} component_mask;

/**
 * ecs.c define as darray<ecs_component_set>.
 **/ 
typedef struct ecs_component_set {
    active_components_array arr;
    
    #define X(arg) arg arg;
    LIST_OF_COMPONENTS
    #undef X
} ecs_component_set;


#define COMPONENT_MASK_OF(typename)  (1ULL << (COMPONENT_MASK_ ## typename))
#define COMPONENT_ID_OF(typename) offsetof(struct ecs_component_set, typename)

// add if u need more then 5
#define COMPONENT_MASK_OF_1(typename) COMPONENT_MASK_OF(typename)
#define COMPONENT_MASK_OF_2(t1, t2) (COMPONENT_MASK_OF(t1) | COMPONENT_MASK_OF(t2)) 
#define COMPONENT_MASK_OF_3(t1, t2, t3) (COMPONENT_MASK_OF(t1) | COMPONENT_MASK_OF(t2) | COMPONENT_MASK_OF(t3))
#define COMPONENT_MASK_OF_4(t1, t2, t3, t4) (COMPONENT_MASK_OF(t1) | COMPONENT_MASK_OF(t2) | COMPONENT_MASK_OF(t3) | COMPONENT_MASK_OF(t4))
#define COMPONENT_MASK_OF_5(t1, t2, t3, t4, t5) (COMPONENT_MASK_OF(t1) | COMPONENT_MASK_OF(t2) | COMPONENT_MASK_OF(t3) | COMPONENT_MASK_OF(t4) | COMPONENT_MASK_OF(t5))

#define COMPONENT_MASK_OF_N(N, ...) CONCAT(COMPONENT_MASK_OF_, N)(__VA_ARGS__)
#define COMPONENT_MASK_OF_ALL(...) COMPONENT_MASK_OF_N(VA_NARGS(__VA_ARGS__), __VA_ARGS__)




#endif 