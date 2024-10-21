#version 330

//we considering that this is already in world coordinates(not local)
//because it is buildin vertices
const vec3 grid_pos[4] = vec3[4](
    vec3(-1.0, 0.0, -1.0),
    vec3( 1.0, 0.0, -1.0),
    vec3( 1.0, 0.0,  1.0),    
    vec3(-1.0, 0.0,  1.0)
);
const int grid_pos_indices[6] = int[6](0, 2, 1, 2, 0, 3);

// projection * world
uniform mat4 world_projection;

void main(){
    int index = grid_pos_indices[gl_VertexID];
    vec4 vertex_pos = vec4((grid_pos[index] * 10.0), 1.0);
    gl_Position = world_projection * vertex_pos;
}