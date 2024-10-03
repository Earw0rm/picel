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
    matrix3f uvn;
    vector3f pos;
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

static bool on_key_press(event_code code, 
                         void* sender,
                         void* listener_inst,
                         event_context data){
    camera c = (camera) listener_inst;
    uint16_t key_code = data.data.ui16[0];

    if(key_code == GLFW_KEY_W){
        camera_move(c,  0.0, -0.1, 0.0);
    }

    if(key_code == GLFW_KEY_S){
        camera_move(c,  0.0, 0.1, 0.0);
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


camera camera_init(){
    camera res = malloc(sizeof(struct camera_mat));
    //TODO crunch
    if(!event_system_register(EVENT_CODE_KEY_PRESSED, res, on_key_press)){
        free(res);
        return nullptr;
    }

    res->uvn = mat3f_id(1);
    res->pos = v3f(0, 0, 0);
    res->speed = 1.0f;
    
    return res;
}

void camera_move(camera camera, float x, float y, float z){
    camera->pos.x += x * camera->speed;
    camera->pos.y += y * camera->speed;
    camera->pos.z += z * camera->speed;    
}

void camera_rotate(camera camera, float ax, float by, float cz){
    camera->uvn = mdotm3(mat3f_rotation(ax, by, cz), camera->uvn);
    // matrix3f rotated = mdotm3(mat3f_rotation(ax, by, cz), camera->uvn);
    
    // float u_magnitude = vec3f_magnitude(rotated.m[0], rotated.m[1], rotated.m[2]);
    // float v_magnitude = vec3f_magnitude(rotated.m[3], rotated.m[4], rotated.m[5]);
    // float n_magnitude = vec3f_magnitude(rotated.m[6], rotated.m[7], rotated.m[8]);    
    
    // rotated.m[0] /=  u_magnitude; 
    // rotated.m[1] /=  u_magnitude;
    // rotated.m[2] /=  u_magnitude;

    // rotated.m[3] /=  v_magnitude;
    // rotated.m[4] /=  v_magnitude;
    // rotated.m[5] /=  v_magnitude;

    // rotated.m[6] /=  n_magnitude;
    // rotated.m[7] /=  n_magnitude;
    // rotated.m[8] /=  n_magnitude;   

    // camera->uvn = rotated;
}

void camera_destroy(camera camera){
    free(camera);
}

matrix4f camera_get_view(camera camera){

    matrix3f scaled = mat3f_id(1);

    float u_magnitude = vec3f_magnitude(camera->uvn.m[0], camera->uvn.m[1], camera->uvn.m[2]);
    float v_magnitude = vec3f_magnitude(camera->uvn.m[3], camera->uvn.m[4], camera->uvn.m[5]);
    float n_magnitude = vec3f_magnitude(camera->uvn.m[6], camera->uvn.m[7], camera->uvn.m[8]);    
    
    scaled.m[0] /=  u_magnitude; 
    scaled.m[1] /=  u_magnitude;
    scaled.m[2] /=  u_magnitude;

    scaled.m[3] /=  v_magnitude;
    scaled.m[4] /=  v_magnitude;
    scaled.m[5] /=  v_magnitude;

    scaled.m[6] /=  n_magnitude;
    scaled.m[7] /=  n_magnitude;
    scaled.m[8] /=  n_magnitude;                            

    matrix4f inverse_uvn = mat4f_from3f(mat3f_transpose(scaled));
    vector3f tmp_pos = mdotv3(scaled, camera->pos);
    inverse_uvn.m[12] =-tmp_pos.x;
    inverse_uvn.m[13] =-tmp_pos.y;
    inverse_uvn.m[14] =-tmp_pos.z;     

    return inverse_uvn;
}