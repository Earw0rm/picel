#version 330 core


in vec2 tex_coord;
in vec4 fragment_in_color;
in vec4 fragment_light_ambient_component;

out vec4 out_color;

uniform bool has_texture; 
uniform sampler2D g_sampler;


void main(){
    //sample texture color using g_sampler. basic color set to (1, 1, 1, 1) (white)
    if(has_texture){
        out_color = fragment_light_ambient_component * texture(g_sampler, tex_coord);
    }else{
        out_color = fragment_light_ambient_component * fragment_in_color;
    }

}