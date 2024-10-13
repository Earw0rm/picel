#ifndef _MATH_TEST_H_
#define _MATH_TEST_H_
#include "yasyc_test.h"
#include "ecs.h"
#include "picel_math.h"


static inline int 
math_basic_quartenion_test(void){
    quaternion id = quaternion_id();
    quaternion r = quaternion_from_axis_angle(DEGREE2RADIANS(90), 0, 1, 0);
    quaternion res = quaternion_normalize(quaternion_mul(r, id));

    ASSERT(r.w == res.w, "multipliing by id quartenion should not change original quartenion");
    ASSERT(r.x == res.x, "multipliing by id quartenion should not change original quartenion");
    ASSERT(r.y == res.y, "multipliing by id quartenion should not change original quartenion");
    ASSERT(r.z == res.z, "multipliing by id quartenion should not change original quartenion");            
    LOG_INFO("[%f %f %f %f]", r.w, r.x, r.y, r.z);

    res = quaternion_normalize(quaternion_mul(id, r));

    ASSERT(r.w == res.w, "multipliing by id quartenion should not change original quartenion");
    ASSERT(r.x == res.x, "multipliing by id quartenion should not change original quartenion");
    ASSERT(r.y == res.y, "multipliing by id quartenion should not change original quartenion");
    ASSERT(r.z == res.z, "multipliing by id quartenion should not change original quartenion");            
    LOG_INFO("[%f %f %f %f]", r.w, r.x, r.y, r.z);

    return 1;
}


static inline void 
math_run_tests(){
    TEST("Math quartenion basic operations test", math_basic_quartenion_test);
}

#endif 