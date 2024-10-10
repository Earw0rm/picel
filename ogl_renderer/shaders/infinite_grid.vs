#version 330 core




const vec3 pos[4] = vec3[4](
    vec3(-1.0, 0.0, -1.0), // bottom left 
    vec3( 1.0, 0.0, -1.0), // bottom rigth
    vec3( 1.0, 0.0,  1.0), // top rigth
    vec3(-1.0, 0.0,  1.0), // top left 
);

const int indices[6] = int[6](0, 2, 1, 2, 0, 3);
out mat4 view;
out mat4 projection;


void main(){
    int index = indices[gl_VertexID];
    vec4 v_pos = vec4(pos[index], 1.0);

    gl_Position = view * projection * v_pos
}

// grid

