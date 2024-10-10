#ifndef _MATH_STRUCT_H_
#define _MATH_STRUCT_H_
/**
 * Column-major (matrix &| vector) operations
*/
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

typedef struct matrix4f { float m[16]; } matrix4f;
typedef struct matrix3f { float m[12]; } matrix3f;

typedef struct quaternion{
    float w; // scalar
    float x, y, z;
}quaternion;

#endif 