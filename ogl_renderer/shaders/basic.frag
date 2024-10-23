#version 330 core
//http://devernay.free.fr/cours/opengl/materials.html
struct material{
    sampler2D texture_specular_0
    sampler2D texture_specular_1
    sampler2D texture_specular_2
    sampler2D texture_specular_3
    sampler2D texture_specular_4

    sampler2D texture_diffuse__0
    sampler2D texture_diffuse__1
    sampler2D texture_diffuse__2
    sampler2D texture_diffuse__3
    sampler2D texture_diffuse__4

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform material mat;

// struct material_v2{
//     sampler2D diffuse; // in all time ambient is equal to diffuse
//     vec3 specular;
//     float shininess;
// };


struct light{
    vec3 position; // position need to be in view coordinates

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 tex_coord;
in vec3 fragment_normals;
in vec3 fragment_position_of_fragment;
out vec4 out_color;

uniform light lig;
uniform bool has_texture; 
uniform sampler2D g_sampler;




void main(){
    vec3 norm = normalize(fragment_normals);
    vec3 light_direction = normalize(/*fragment_light_source_position*/lig.position - fragment_position_of_fragment);         
    vec4 diffusion_component = vec4(mat.diffuse, 1) * max(dot(norm, light_direction), 0) * vec4(lig.diffuse, 1)/*fragment_light_ambient_component*/;
    

    // sinse in view coordinates
    vec3 view_direction = normalize(/*view_position*/-fragment_position_of_fragment); 
    vec3 reflection_dir = reflect(-light_direction, norm);
    float spec = pow(max(dot(view_direction, reflection_dir), 0.0), mat.shininess);
    vec4 specular_component =  vec4(mat.specular, 1) * spec * vec4(lig.specular, 1);

    vec4 ambient_component = vec4(mat.ambient, 1) * vec4(lig.ambient, 1); 
    
    //sample texture color using g_sampler. basic color set to (1, 1, 1, 1) (white)
    if(has_texture){
        out_color = (ambient_component + diffusion_component + specular_component) * texture(g_sampler, tex_coord);
    }else{
        out_color = (ambient_component + diffusion_component + specular_component);
    }

}