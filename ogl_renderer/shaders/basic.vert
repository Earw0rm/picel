#version 330 core
// declarations of input attributes in shader
// we choose hire index of vertex attributes
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec4 inColors;

//https://en.wikibooks.org/wiki/GLSL_Programming/Vector_and_Matrix_Operations



// @vecN
//   - float
// i - int
// u - unsigned int 
// d - double
// b - bool 
// N - 1, 2, 3, 4 (vector size)

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertexColor;



void main(){
    gl_Position = projection * view * model * vec4(in_pos, 1.0);
    vertexColor = inColors;       
}