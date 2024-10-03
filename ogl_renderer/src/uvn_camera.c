#include "uvn_camera.h"
#include <stdlib.h>
#include "memory.h"
#include "math.h"
#include "event_system.h"
#include <GLFW/glfw3.h>




struct camera_mat{
/**
 * Original UVM (u right, v up, m back)
 * -------------------------
 * | u v n |.T -dot(u, px) |
 * | u v n |   -dot(v, py) |
 * | u v n |   -dot(n, pz) |
 * -------------------------
 * | 0 0 0 |   1           |
 * -------------------------
 **/
    float speed;

    vector3f world_position;

    // actually pointing in the reverse direction

    vector3f direction; // v
    vector3f up; // v
    vector3f right; // u
};



/**
 * Notes about future implementation
 * 1) we need 3 vector - m_pos, m_target, m_up
 * @param m_pos    -> position of the camera in world coordinate systems
 * @param m_target -> direction that camera looking up (N in UVN). By default target is -z axis(opengl style)
 * @param m_up     -> up of the camera (V in UVN)
 * 2) third vector, V, we can calculate on the fly
 * 
 *  
 * 
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

static void 
camera_move(camera camera, float x, float y, float z){
    camera->world_position.x += x * camera->speed;
    camera->world_position.y += y * camera->speed;
    camera->world_position.z += z * camera->speed;    
}

static bool 
on_key_press(event_code code, 
                         void* sender,
                         void* listener_inst,
                         event_context data){
    camera c = (camera) listener_inst;
    uint16_t key_code = data.data.ui16[0];

    if(key_code == GLFW_KEY_W){
        camera_move(c,  0.0, 0.0, -0.1);
    }

    if(key_code == GLFW_KEY_S){
        camera_move(c,  0.0, 0.0, 0.1);
    }

    if(key_code == GLFW_KEY_A){
        camera_move(c, 0.1, 0.0, 0.0);
    }

    if(key_code == GLFW_KEY_D){
        camera_move(c, -0.1, 0.0, 0.0);
    }

    if(key_code == GLFW_KEY_UP){
        camera_rotate(c, 1.0, 0, 0);
    }

    if(key_code == GLFW_KEY_DOWN){
        camera_rotate(c, -1.0, 0, 0);
    }

    if(key_code == GLFW_KEY_LEFT){
        camera_rotate(c, 0.0, 1.0, 0.0);
    }

    if(key_code == GLFW_KEY_RIGHT){
        camera_rotate(c, 0, -1.0, 0.0);
    }   

    return false;
}
void camera_set_position(camera c, float x, float y, float z){
    c->world_position = v3f(x, y, z);   
}

camera camera_init(vector3f camera_position,
                   vector3f camera_target,
                   vector3f world_space_up /* |0, 1, 0| by default */){
    camera res = malloc(sizeof(struct camera_mat));
    //TODO crunch
    if(!event_system_register(EVENT_CODE_KEY_PRESSED, res, on_key_press)){
        free(res);
        return nullptr;
    }
    // should be in math library?
    // https://en.wikipedia.org/wiki/Gram-Schmidt_process
    res->world_position = camera_position;
    res->direction = vec3f_normalize(vec3f_diff(camera_position, camera_target));
    res->right = vec3f_normalize(vec3f_cross(world_space_up, res->direction));
    res->up = vec3f_cross(res->direction, res->right);

    res->speed = 1.0f;

    
    return res;
}


void camera_rotate(camera camera, float ax, float by, float cz){


}

void camera_destroy(camera camera){
    free(camera);
}

matrix4f camera_get_view(camera camera){

    matrix4f a;
    matrix4f b = mat4f_id(1);
    a.m[0] = camera->right.x; 
    a.m[1] = camera->up.x;                       
    a.m[2] = camera->direction.x;
    a.m[3] = 0;

    a.m[4] = camera->right.y;
    a.m[5] = camera->up.y;                        
    a.m[6] = camera->direction.y;
    a.m[7] = 0;

    a.m[8]  = camera->right.z;
    a.m[9]  = camera->up.z;                       
    a.m[10] = camera->direction.z;
    a.m[11] = 0;

    a.m[12] = 0;
    a.m[13] = 0;                       
    a.m[14] = 0;
    a.m[15] = 1;

    b.m[12] = -camera->world_position.x;
    b.m[13] = -camera->world_position.y;
    b.m[14] = -camera->world_position.z;
    
    return mdotm4(a, b);
}