#ifndef _MATH_H_
#define _MATH_H_
#include <math.h>
#include "logger.h"
#define PI 3.14159265f
#define RADIANS2DEGREE(rad) (rad * (180.0f / PI))
#define DEGREE2RADIANS(deg) ((deg) * PI / 180.0f)

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

static inline void 
log_matrix(matrix4f m){

    LOG_INFO("| %f %f %f %f |", m.m[0], m.m[4], m.m[8], m.m[12]);
    LOG_INFO("| %f %f %f %f |", m.m[1], m.m[5], m.m[9], m.m[13]);
    LOG_INFO("| %f %f %f %f |", m.m[2], m.m[6], m.m[10], m.m[14]);
    LOG_INFO("| %f %f %f %f |", m.m[3], m.m[7], m.m[11], m.m[15]);
    LOG_INFO("===================================================");
}

static inline matrix4f 
mat4f_id(const float f){
    matrix4f ret = { f, 0, 0, 0,
                     0, f, 0, 0,
                     0, 0, f, 0,
                     0, 0, 0, f };
    return ret;
}

static inline matrix3f 
mat3f_id(const float f){
    matrix3f ret = { f, 0, 0, 
                     0, f, 0,
                     0, 0, f};
    return ret;
}

static inline vector4f 
v4f_id(const float f){
    //todo ret = {f};
    vector4f ret = {.x = f, .y = f, .z = f, .w = f};
    return ret;
}

static inline vector4f 
v4f(const float x, const float y, const float z, const float w){
    vector4f ret = {.x = x, .y = y, .z = z, .w = w};
    return ret;
}

static inline vector3f 
v3f_id(const float f){
    vector3f ret = {.x = f, .y = f, .z = f};
    return ret;
}

static inline vector3f 
v3f(const float x, const float y, const float z){
    vector3f ret = {.x = x, .y = y, .z = z};
    return ret;
}

static inline vector2f
v2f_id(const float f){
    vector2f ret = {.x = f, .y = f};
    return ret; 
}

static inline vector2f
v2f(const float x, const float y){
    vector2f ret = {.x = x, .y = y};
    return ret; 
}

static inline vector4f 
mdotv4(matrix4f m, vector4f v){

    vector4f ret = v4f(
        (m.m[0] * v.x) + (m.m[4] * v.y) + (m.m[8] * v.z) + (m.m[12] * v.w),
        (m.m[1] * v.x) + (m.m[5] * v.y) + (m.m[9] * v.z) + (m.m[13] * v.w),
        (m.m[2] * v.x) + (m.m[6] * v.y) + (m.m[10] * v.z) + (m.m[14] * v.w),
        (m.m[3] * v.x) + (m.m[7] * v.y) + (m.m[11] * v.z) +(m.m[15] * v.w)
    );
    
    return ret;
}   

static inline matrix4f 
mdotm4(matrix4f l, matrix4f r){
    matrix4f res;
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            res.m[col * 4 + row] = l.m[0 * 4 + row] * r.m[col * 4 + 0]
                                 + l.m[1 * 4 + row] * r.m[col * 4 + 1]
                                 + l.m[2 * 4 + row] * r.m[col * 4 + 2]
                                 + l.m[3 * 4 + row] * r.m[col * 4 + 3];
        }
    }
    return res;
}

static inline matrix4f
mat4f_transpose(matrix4f m){
    matrix4f res = {0};
    res.m[0]  = m.m[0];
    res.m[1]  = m.m[4];
    res.m[2]  = m.m[8];
    res.m[3]  = m.m[12];
    res.m[4]  = m.m[1];
    res.m[5]  = m.m[5];
    res.m[6]  = m.m[9];
    res.m[7]  = m.m[13];
    res.m[8]  = m.m[2];
    res.m[9]  = m.m[6];
    res.m[10] = m.m[10];
    res.m[11] = m.m[14];
    res.m[12] = m.m[3];
    res.m[13] = m.m[7];
    res.m[14] = m.m[11];
    res.m[15] = m.m[15];    
    return res;                                                        
}

static inline matrix4f 
mat4f_translation(float x, float y, float z){
    matrix4f id = mat4f_id(1);
    id.m[12]  = x;
    id.m[13]  = y;
    id.m[14]  = z;
    return id;
}

// expect angle in degree 
static inline matrix4f
mat4f_rotation(float ax, float by, float cz){
    matrix4f res = mat4f_id(1);
    if(cz != 0){
        float rad_cz = DEGREE2RADIANS(cz);
        matrix4f czrot = mat4f_id(1);
        czrot.m[0]  =  cos(rad_cz);
        czrot.m[1]  =  sin(rad_cz);
        czrot.m[4]  = -sin(rad_cz);
        czrot.m[5]  =  cos(rad_cz);
        res = mdotm4(czrot, res);
    }
    if(by != 0){
        float rad_by = DEGREE2RADIANS(by);
        matrix4f byrot = mat4f_id(1);
        byrot.m[0]  = cos(rad_by);
        byrot.m[2]  = -sin(rad_by);
        byrot.m[8]  = sin(rad_by);
        byrot.m[10] = cos(rad_by);
        res = mdotm4(byrot, res);
    }
    if(ax != 0){
        float rad_ax = DEGREE2RADIANS(ax);
        matrix4f axrot = mat4f_id(1);
        axrot.m[5]  = cos(rad_ax);
        axrot.m[6]  = sin(rad_ax);
        axrot.m[9]  = -sin(rad_ax);
        axrot.m[10] = cos(rad_ax);
        res = mdotm4(axrot, res);
    }

    return res;
}

static inline matrix4f
math4f_diag(float x, float y, float z){
    matrix4f res = mat4f_id(1);
    res.m[0] = x;
    res.m[5] = y;
    res.m[10] = z;
    return res;   
}
static inline matrix4f
mat4f_scale(matrix4f m, float ax, float by, float cz){
    matrix4f res = mdotm4(
        math4f_diag(ax, by, cz), m
    );


    return res;
}

static inline matrix4f
mat4f_rotate(matrix4f m, float ax, float by, float cz){
    matrix4f res = mdotm4(
        mat4f_rotation(ax, by, cz), m
    );
    return res; 
}

static inline matrix4f 
mat4f_translate(matrix4f m, float x, float y, float z){
   return mdotm4(
     mat4f_translation(x, y, z), m
   );
}



#endif 