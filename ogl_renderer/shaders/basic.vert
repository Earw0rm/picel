#version 330 core
// declarations of input attributes in shader
// we choose hire index of vertex attributes
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_tex_coord;
layout (location = 2) in vec4 in_color;

layout (location = 3) in vec4 in_light_ambient_component;
//https://en.wikibooks.org/wiki/GLSL_Programming/Vector_and_Matrix_Operations


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 tex_coord;
out vec4 fragment_in_color;
out vec4 fragment_light_ambient_component;

void main(){

    gl_Position =  projection * view * model * vec4(in_pos, 1.0);
    tex_coord = in_tex_coord;
    fragment_in_color = in_color;
    fragment_light_ambient_component = in_light_ambient_component;
}