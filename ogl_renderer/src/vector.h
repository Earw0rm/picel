#ifndef _VECTOR_H_
#define _VECTOR_H_


typedef struct vector2f{
    float x;
    float y;
}vector2f;

typedef struct vector3f{
    float x;
    float y;
    float z;
}vector3f;

typedef struct vector4f{
    float x;
    float y;
    float z;
    float w;
}vector4f;

static inline vector4f 
v4f(const float f){
    vector4f ret = {.x = f, .y = f, .z = f, .w = f};
    return ret;
}

static inline vector3f 
v3f(const float f){
    vector3f ret = {.x = f, .y = f, .z = f};
    return ret;
}

static inline vector2f
v2f(const float f){
    vector2f ret = {.x = f, .y = f};
    return ret; 
}


#endif 