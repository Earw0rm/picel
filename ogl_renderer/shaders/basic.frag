#version 330 core


in vec2 tex_coord;

out vec4 out_color;

uniform sampler2D g_sampler;


void main(){

    //sample texture color using g_sampler
    out_color = texture2D(g_sampler, tex_coord);
}