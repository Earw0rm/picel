#include "render_system.h"
#include "containers/darray.h"
#include "picel_math.h"

#include "mesh2.h"
#include "texture2.h"
#include "scene.h"


void render_system_render(ecs ecs, window w, camera main_camera, shader gs, GLuint dvao){
    // matrix4f view = camera_get_view(main_camera);

    float aspect_ratio = win_get_aspect_ratio(w);
    matrix4f projection = mat4f_projection(20.0f, aspect_ratio, 1, 100);
    matrix4f view = win_get_view(w);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //ambient component light
    glVertexAttrib4f(3, 1, 1, 1, 1.0);
    
    //todo crunch
    vector3f light_source_position = v3f(-2, 2, -5);
    vector3f light_ambient         = v3f(1, 1, 1);
    vector3f light_diffuse         = v3f(1, 1, 1);
    vector3f light_specular        = v3f(1, 1, 1);        


    // // draw grid    
    // GLenum err;
    // matrix4f vp = mdotm4(projection, view);

    // glUseProgram(gs.program);
    // glBindVertexArray(dvao);

    // GLint vp_loc = glGetUniformLocation(gs.program, "world_projection");
    // if(vp_loc == -1){
    //     LOG_FATAL("failed to find wolrd_projection matrix inside grid shader");
    // }                

    // glUniformMatrix4fv(vp_loc, 1, GL_FALSE, vp.m);
    // if((err = glGetError()) != GL_NO_ERROR){
    //     LOG_FATAL("Failed to draw grid %i", err);
    // }

    // glDrawArrays(GL_TRIANGLES, 0, 6);
    // if((err = glGetError()) != GL_NO_ERROR){
    //     LOG_FATAL("Failed to draw grid %x", err);
    // }
    // glBindVertexArray(0);
    // // LOG_INFO("draw successfull?");


    darray qres = ecs_query(ecs, component_transform, component_mesh, component_material, component_renderable);
    for(uint64_t i = 0; i < darray_lenght(qres); ++i){
        component_transform* ctrans = ecs_get_component(ecs, i, component_transform);
        component_mesh* cmesh = ecs_get_component(ecs, i, component_mesh);
        component_material* cmat = ecs_get_component(ecs, i, component_material);
        
        glUseProgram(cmat->shader_program);
        glBindVertexArray(cmesh->vao);        


        // sinse we using vao we dont need to activate this
        // glBindBuffer(GL_ARRAY_BUFFER, cmesh->vbo);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cmesh->ebo);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        bool has_texture = cmat->texture_obj > 0;
        GLint has_texture_loc  = glGetUniformLocation(cmat->shader_program, "has_texture");        
        glUniform1i(has_texture_loc, has_texture); 
        if(has_texture){
            // textures section
            glEnableVertexAttribArray(1);
            glActiveTexture(GL_TEXTURE0); // Specifies which texture unit to make active.
            glBindTexture(GL_TEXTURE_2D, cmat->texture_obj); // today support only one texture
            GLint g_sampler_loc  = glGetUniformLocation(cmat->shader_program, "g_sampler");
            glUniform1i(g_sampler_loc, 0); // 0 index of texture unit, mean that we say that this samples uses GL_TEXTURE0
        }

        GLint model_loc      = glGetUniformLocation(cmat->shader_program, "model");
        GLint view_loc       = glGetUniformLocation(cmat->shader_program, "view");
        GLint projection_loc = glGetUniformLocation(cmat->shader_program, "projection");    

        //material structure inside fragment shader
        GLint mat_ambient_loc   = glGetUniformLocation(cmat->shader_program, "mat.ambient");
        if(mat_ambient_loc == -1){
            LOG_WARN("cannot find mat_ambient_location");
        }
        GLint mat_diffuse_loc   = glGetUniformLocation(cmat->shader_program, "mat.diffuse");
        if(mat_diffuse_loc == -1){
            LOG_WARN("cannot find mat_diffuse_location");
        }

        GLint mat_specular_loc  = glGetUniformLocation(cmat->shader_program, "mat.specular");
        if(mat_specular_loc == -1){
            LOG_WARN("cannot find mat_specular_location");
        }

        GLint mat_shininess_loc = glGetUniformLocation(cmat->shader_program, "mat.shininess");                
        if(mat_shininess_loc == -1){
            LOG_WARN("cannot find mat_shininess_locaction");
        }

        GLint light_position_loc = glGetUniformLocation(cmat->shader_program, "lig.position");                
        if(light_position_loc == -1){
            LOG_WARN("cannot find light position");
        }

        GLint light_ambient_loc = glGetUniformLocation(cmat->shader_program, "lig.ambient");                
        if(light_ambient_loc == -1){
            LOG_WARN("cannot find light_ambient_location");
        }

        GLint light_diffuse_loc = glGetUniformLocation(cmat->shader_program, "lig.diffuse");                
        if(light_diffuse_loc == -1){
            LOG_WARN("cannot find light_diffuse_location");
        }

        GLint light_specular_loc = glGetUniformLocation(cmat->shader_program, "lig.specular");                
        if(light_specular_loc == -1){
            LOG_WARN("cannot find light_specular_location");
        }




        matrix4f model = mat4f_id(1);
        model = mat4f_scale(model, ctrans->scale.x, ctrans->scale.y, ctrans->scale.z);
        model = mat4f_rotate(model, ctrans->rotation.x, ctrans->rotation.y, ctrans->rotation.z);
        model = mat4f_translate(model, ctrans->position.x, ctrans->position.y, ctrans->position.z);      

        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.m);        
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, projection.m);
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.m);


        glUniform3f(mat_ambient_loc,
                    cmat->ambient.x,
                    cmat->ambient.y,
                    cmat->ambient.z);

        glUniform3f(mat_diffuse_loc,
                    cmat->diffuse.x,
                    cmat->diffuse.y,
                    cmat->diffuse.z);

        glUniform3f(mat_specular_loc,
                    cmat->specular.x,
                    cmat->specular.y,
                    cmat->specular.z);

        glUniform1f(mat_shininess_loc, cmat->shininnes);


        vector4f tmp_lpos = v4f(light_source_position.x, light_source_position.y, light_source_position.z, 1.0);
        tmp_lpos = mdotv4(view, mdotv4(model, tmp_lpos));
        light_source_position.x = tmp_lpos.x;
        light_source_position.y = tmp_lpos.y;
        light_source_position.z = tmp_lpos.z;

        // light source position
        glUniform3f(light_position_loc,
                    light_source_position.x,
                    light_source_position.y,
                    light_source_position.z);
        
        glUniform3f(light_ambient_loc,
                    light_ambient.x,
                    light_ambient.y,
                    light_ambient.z);

        glUniform3f(light_diffuse_loc,
                    light_diffuse.x,
                    light_diffuse.y,
                    light_diffuse.z);

        glUniform3f(light_specular_loc,
                    light_specular.x,
                    light_specular.y,
                    light_specular.z);
        
        glDrawElements(GL_TRIANGLES, cmesh->vectex_count, GL_UNSIGNED_INT, 0 /*last parameter deprecated*/);
        glBindVertexArray(0);
        glUseProgram(0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        //if our value defined as glVertexAttrib4v, then we dont need to enable or disable atribArray
        // glDisableVertexAttribArray(3);
    } 



}


// renderer start
void render_mesh(mesh mesh, shader sh){
    glUseProgram(sh.program); // if it is not used yet
    mesh_to_gpu(mesh);


    mesh_activate_textures(mesh, sh);
    glActiveTexture(GL_TEXTURE0);

    //draw
    glBindVertexArray(mesh_vao(mesh));
    glDrawElements(GL_TRIANGLES, mesh_indices_len(mesh), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void render_scene(shader sh, scene s){
    darray meshes = scene_get_meshes(s);
    for(int i = 0; i < darray_lenght(meshes), ++i){
        mesh m = (mesh) darray_at(meshes, i);
        render_mesh(m, sh);
    }
}