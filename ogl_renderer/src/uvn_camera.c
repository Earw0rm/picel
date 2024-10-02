#include "uvn_camera.h"
#include <stdlib.h>
#include "memory.h"
#include "math.h"

struct camera_mat{
    vector4f u; // right 
    vector4f v; // up
    vector4f m; // back


    matrix4f value;
};



/**
 * Notes about implementation
 * 1) we need 3 vector - m_pos, m_target, m_up
 * @param m_pos    -> position of the camera in world coordinate systems
 * @param m_target -> direction that camera looking up (N in UVN). By default target is -z axis(opengl style)
 * @param m_up     -> up of the camera (V in UVN)
 * 2) third vector, V, we can calculate on the fly
 * 
 *  
 * TODO crunch
 * @param m_speed 1.0f we need a speed parameter for every object in our scene.
 * @param event_system we need subscribe on_keyboard and on_mouse event
 * 
 * 
 * 
 * next, how we handle move
 * 
 * if camera it is first place person then
 * UP   -> m_pos  += (target * speed)
 * DOWN -> m_pos  += (target * speed)
 * 
 * c=axb -> iff a and b linear independent vector, then c is vector that
 * is perpendicular to both a and b
 * for example, if we have x, y basis vector, then xxy -> z vector
 * 
 * basic properties
 * axb = -bxa
 * ax(b+c) = axb + axc
 * 
 * 
 * INTUITION - we have vector that direct to the target, and up.
 * if we need move to the left, we need a third vector that direct to the left
 * relative to our two vectors. 
 * 
 * 
 * LEFT  -> {
 * l = cross_product(m_target, m_up)
 * l = normalize(l) * m_speed  // we dont need to normalize if m_target and m_up is unit vector
 * m_pos +=l
 * }
 * 
 * RIGHT -> {
 * l = cross_product(m_up, m_target)
 * l = normalize(l) * m_speed
 * }
 * 
 * 
 *Vec3 cross_product(Vec3 a, Vec3 b) {
 *   Vec3 result;
 *   result.x = a.y * b.z - a.z * b.y;
 *   result.y = a.z * b.x - a.x * b.z;
 *   result.z = a.x * b.y - a.y * b.x;
 *   return result;
 *}
 * 
 * 
 * all transformation need to be lazy, until we call getMatrix or something like this
 * getMatrix take our 3 vectors and get our matrix. Functional of getMatrix is the same
 * as glm::lookAt.
 */


camera camera_init(){
    camera res = malloc(sizeof(struct camera_mat));
    res->value = mat4f_id(1); 
    return res;
}

void camera_move(camera camera, float x, float y, float z){
    camera->value.m[12] += -x;
    camera->value.m[13] += -y;
    camera->value.m[14] += -z;
}

void camera_destroy(camera camera){
    free(camera);
}

matrix4f camera_get_data(camera camera){
    return camera->value;
}