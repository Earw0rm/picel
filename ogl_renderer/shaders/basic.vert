#version 330 core
// declarations of input attributes in shader
// we choose hire index of vertex attributes
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_tex_coord;
layout (location = 2) in vec3 in_normals;
//https://en.wikibooks.org/wiki/GLSL_Programming/Vector_and_Matrix_Operations


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 tex_coord;
out vec4 fragment_in_color;
out vec3 fragment_normals;
out vec3 fragment_position_of_fragment;
void main(){

    gl_Position =  projection * view * model * vec4(in_pos, 1.0);
    tex_coord = in_tex_coord;

    fragment_position_of_fragment = vec3(view * model * vec4(in_pos, 1.0));

                       // dont do inverse matrix inside shader
                       // change normal vector orientation to world coordinates
    fragment_normals = mat3(transpose(inverse(view * model))) * in_normals;
}