#ifndef _MATH_H_
#define _MATH_H_
#include <math.h>
#include <stdint.h>
#include "logger.h"
#include "math_struct.h"
#include <string.h>


#include <cglm/cglm.h>   /* for inline */

#define PI 3.14159265f
#define RADIANS2DEGREE(rad) (rad * (180.0f / PI))
#define DEGREE2RADIANS(deg) ((deg) * PI / 180.0f)


static inline void 
log_matrix4f(matrix4f m){

    LOG_INFO("| %f %f %f %f |", m.m[0], m.m[4], m.m[8], m.m[12]);
    LOG_INFO("| %f %f %f %f |", m.m[1], m.m[5], m.m[9], m.m[13]);
    LOG_INFO("| %f %f %f %f |", m.m[2], m.m[6], m.m[10], m.m[14]);
    LOG_INFO("| %f %f %f %f |", m.m[3], m.m[7], m.m[11], m.m[15]);
    LOG_INFO("===================================================");
}

static inline void 
log_matrix3f(matrix3f m){

    LOG_INFO("| %f %f %f|", m.m[0], m.m[3], m.m[6]);
    LOG_INFO("| %f %f %f|", m.m[1], m.m[4], m.m[7]);
    LOG_INFO("| %f %f %f|", m.m[2], m.m[5], m.m[8]);
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

static inline vector3f 
mdotv3(matrix3f m, vector3f v){

    vector3f ret = v3f(
        (m.m[0] * v.x) + (m.m[3] * v.y) + (m.m[6] * v.z),
        (m.m[1] * v.x) + (m.m[4] * v.y) + (m.m[7] * v.z),
        (m.m[2] * v.x) + (m.m[5] * v.y) + (m.m[8] * v.z)
    );
    
    return ret;
}   

static inline matrix4f
mdotm4(matrix4f l, matrix4f r) {
    matrix4f res;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            res.m[col * 4 + row] = 
                l.m[0 * 4 + row] * r.m[col * 4 + 0] +
                l.m[1 * 4 + row] * r.m[col * 4 + 1] +
                l.m[2 * 4 + row] * r.m[col * 4 + 2] +
                l.m[3 * 4 + row] * r.m[col * 4 + 3];
        }
    }
    return res;
}

static inline matrix3f
mdotm3(matrix3f l, matrix3f r) {
    matrix3f res;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            res.m[col * 3 + row] = 
                l.m[0 * 3 + row] * r.m[col * 3 + 0] +
                l.m[1 * 3 + row] * r.m[col * 3 + 1] +
                l.m[2 * 3 + row] * r.m[col * 3 + 2];
        }
    }
    return res;
}


static inline matrix4f
mat4f_transpose(matrix4f m){
    matrix4f res;
    uint8_t i = 0;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            res.m[i] = m.m[row + col * 4];
            ++i;
        }
    }
    return res;
}

static inline matrix3f
mat3f_transpose(matrix3f m){

    matrix3f res;
    uint8_t i = 0;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            res.m[i] = m.m[row + col * 3];
            ++i;
        }
    }

    return res;
}

static inline vector3f
vec3f_cross(vector3f a, vector3f b) {
    vector3f res = {
        .x = a.y * b.z - a.z * b.y,
        .y = a.z * b.x - a.x * b.z,
        .z = a.x * b.y - a.y * b.x
    };
    return res;
}

static inline vector4f 
vec4f_from3f(vector3f v){
    vector4f res = {
        .x = v.x,
        .y = v.y,
        .z = v.z,
        .w = 1
    };
    return res;
}

static inline matrix4f 
mat4f_from3f(matrix3f m){
    matrix4f res = mat4f_id(1);
    res.m[0]  = m.m[0];
    res.m[1]  = m.m[1];
    res.m[2]  = m.m[2];

    res.m[4]  = m.m[3];
    res.m[5]  = m.m[4];
    res.m[6]  = m.m[5];

    res.m[8]  = m.m[6];
    res.m[9]  = m.m[7];
    res.m[10] = m.m[8];    
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

// expect angle in degree 
static inline matrix3f
mat3f_rotation(float ax, float by, float cz){
    matrix3f res = mat3f_id(1);
    if(cz != 0){
        float rad_cz = DEGREE2RADIANS(cz);
        matrix3f czrot = mat3f_id(1);
        czrot.m[0]  =  cos(rad_cz);
        czrot.m[1]  =  sin(rad_cz);
        czrot.m[4]  = -sin(rad_cz);
        czrot.m[5]  =  cos(rad_cz);
        res = mdotm3(czrot, res);
    }
    if(by != 0){
        float rad_by = DEGREE2RADIANS(by);
        matrix3f byrot = mat3f_id(1);
        byrot.m[0]  = cos(rad_by);
        byrot.m[2]  = -sin(rad_by);
        byrot.m[8]  = sin(rad_by);
        byrot.m[10] = cos(rad_by);
        res = mdotm3(byrot, res);
    }
    if(ax != 0){
        float rad_ax = DEGREE2RADIANS(ax);
        matrix3f axrot = mat3f_id(1);
        axrot.m[5]  = cos(rad_ax);
        axrot.m[6]  = sin(rad_ax);
        axrot.m[9]  = -sin(rad_ax);
        axrot.m[10] = cos(rad_ax);
        res = mdotm3(axrot, res);
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

static inline vector3f
vec3f_diff(vector3f l, vector3f r){
    vector3f res = {
        .x = l.x - r.x,
        .y = l.y - r.y,
        .z = l.z - r.z
    };
    return res;
}

static inline vector3f
vec3f_summ(vector3f l, vector3f r){
    vector3f res = {
        .x = l.x + r.x,
        .y = l.y + r.y,
        .z = l.z + r.z
    };
    return res;
}

static inline vector3f
vec3f_scalar_mul(vector3f l, float scalar){
    vector3f res = {
        .x = l.x * scalar,
        .y = l.y * scalar,
        .z = l.z * scalar
    };
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

static inline float
vec4f_magnitude(vector4f v){
    return sqrtf(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2));
}

static inline float
vec3f_magnitude(float x, float y, float z){
    return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
}
static inline vector3f
vec3f_normalize(vector3f v){
    float magniture = vec3f_magnitude(v.x, v.y, v.z);
    vector3f res = {
        .x = v.x / magniture, 
        .y = v.y / magniture,
        .z = v.z / magniture
    };
    return res;
}

static inline matrix4f
mat4f_from3fv(vector3f a, vector3f b, vector3f c){
    matrix4f res = mat4f_id(1);
    res.m[0] = a.x;
    res.m[1] = a.y;
    res.m[2] = a.z;

    res.m[4] = b.x;
    res.m[5] = b.y;
    res.m[6] = b.z;

    res.m[8] = b.x;
    res.m[9] = b.y;
    res.m[10]= b.z;

    return res;
}

static inline matrix4f
mat4f_t_from3fv(vector3f a, vector3f b, vector3f c){
    matrix4f res = mat4f_id(1);
    res.m[0] = a.x;
    res.m[4] = a.y;
    res.m[8] = a.z;

    res.m[1] = b.x;
    res.m[5] = b.y;
    res.m[9] = b.z;

    res.m[2] = b.x;
    res.m[6] = b.y;
    res.m[10]= b.z;
    return res;
}


static inline matrix4f
mat4f_projection(float hfov, float ar, float nearz, float farz) {

    float tanHalfFOV = tanf(DEGREE2RADIANS(hfov / 2));
    float f = 1.0f / tanHalfFOV;
    
    float val_33 = -(farz + nearz) / (farz - nearz );
    float val_43 = -(2 * farz * nearz) / (farz - nearz );

    matrix4f res = {
        (f / ar), 0,     0,      0,
        0,        f,     0,      0,
        0,        0,     val_33, -1,
        0,        0,     val_43,  0
    };

    return res;
}

// angle in degree 
static inline matrix4f
mat4f_project(matrix4f m, float hfov, float aspect_ratio, float nearz, float farz){
    
    matrix4f res = mdotm4(mat4f_projection(hfov, aspect_ratio, nearz, farz), m); 

    return res;
    
}

static inline matrix4f
mat4f_from_cglm(mat4 m){
    matrix4f res;
    memcpy(&res.m, m, sizeof(float)* 16);
    return res;
}

static inline quaternion
quaternion_from_axis_angle(float angle, float ux, float uy, float uz){
    float cos_half_angle = cosf(angle / 2);
    float sin_half_angle = sinf(angle / 2);
    quaternion res = {
        .w = cos_half_angle,
        .x = ux * sin_half_angle, 
        .y = uy * sin_half_angle, 
        .z = uz * sin_half_angle
    };
    return res;
}


static inline quaternion
quaternion_conjugate(quaternion q){
    quaternion res = {
        .w = q.w,
        .x = -q.x,
        .y = -q.y,
        .z = -q.z
    };
    return res;
}

static inline quaternion
quaternion_from_vec3f(vector3f v){
    quaternion res = {
        .w = 0,
        .x = v.x,
        .y = v.y,
        .z = v.z
    };
    return res;
}

static inline quaternion
quaternion_mul(quaternion q1, quaternion q2){
    quaternion res = {
        .w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
        .x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
        .y = q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
        .z = q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x
    };
    return res;
}

static inline vector3f
quaternion_point_rotate(quaternion q, vector3f v){
    quaternion q_conj = quaternion_conjugate(q);
    quaternion q_res = quaternion_mul(
        quaternion_mul(q, quaternion_from_vec3f(v)), q_conj
    );
    vector3f v_res = {
        .x = q_res.x,
        .y = q_res.y,
        .z = q_res.z
    };
    return v_res;
}


/**
 * https://chanhaeng.blogspot.com/2018/09/quaternion-camera-implementation.html
 * @param world_camera_position position of the camera in the world coordinates
 * @param world_target target/position the camera should look at
 * @param up vector pointing to the positive y direction, used to create right direction vector
 */
static inline matrix4f
look_at(vector3f world_camera_position, vector3f  world_target, vector3f up){
    matrix4f pos = mat4f_id(1);
    pos.m[12] = -world_camera_position.x;
    pos.m[13] = -world_camera_position.y;
    pos.m[14] = -world_camera_position.z;

    vector3f direction = vec3f_normalize(vec3f_diff(world_target, world_camera_position));
    vector3f right = vec3f_normalize(vec3f_cross(up, direction));
    vector3f nup = vec3f_normalize(vec3f_cross(direction, right));     
    return mdotm4(mat4f_t_from3fv(right, nup, direction), pos);
}

static inline quaternion
quartenion_normalize(quaternion q){
    float norm = sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
    q.w /= norm;
    q.x /= norm;
    q.y /= norm;
    q.z /= norm;
    return q;
}

/**
 * https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#Quaternion-derived_rotation_matrix
 * @param world_camera_position position of the camera in the world coordinates
 * @param q quartenion that describe rotation
 */
static inline matrix4f
quartenion_look_at(vector3f position, quaternion q){
    matrix4f pos = mat4f_id(1);
    matrix4f r = mat4f_id(1);

    pos.m[12] = -position.x;
    pos.m[13] = -position.y;
    pos.m[14] = -position.z;

    float xx = q.x * q.x;
    float yy = q.y * q.y;
    float zz = q.z * q.z;

    float xy = q.x * q.y;
    float xz = q.x * q.z;
    float yz = q.y * q.z;

    float wx = q.w * q.x;
    float wy = q.w * q.y;
    float wz = q.w * q.z;
    
    // LOG_INFO("yy + zz %f", zz);
    // LOG_INFO("xx + zz %f", zz);

    r.m[0] = 1.0f - 2.0f * (yy + zz);
    r.m[1] = 2.0f * (xy + wz);
    r.m[2] = 2.0f * (xz - wy);  

    r.m[4] = 2.0f * (xy - wz);
    r.m[5] = 1.0 - 2.0f * (xx + zz);
    r.m[6] = 2.0f * (yz + wx);

    r.m[8] = 2.0f * (xz + wy);
    r.m[9] = 2.0f * (yz - wx);
    r.m[10] = 1.0f - 2.0f * (xx + yy);

    // LOG_INFO("r matrix");
    // log_matrix4f(r);
    // LOG_INFO("pos matrix");
    // log_matrix4f(pos);

    return mdotm4(r, pos);
}

#endif 