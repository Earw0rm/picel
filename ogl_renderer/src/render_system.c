#include "render_system.h"
#include "containers/darray.h"
#include "picel_math.h"


void render_system_render(ecs ecs, window w, camera main_camera){
    // matrix4f view = camera_get_view(main_camera);

    float aspect_ratio = win_get_aspect_ratio(w);
    matrix4f projection = mat4f_projection(20.0f, aspect_ratio, 1, 100);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //ambient component light
    glVertexAttrib4f(3, 1, 1, 1, 1.0);

    vector3f light_source_position = v3f(-2, 2, -5);

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
        GLint light_source_position_loc = glGetUniformLocation(cmat->shader_program, "light_source_position");

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


        matrix4f view = win_get_view(w);
        matrix4f model = mat4f_id(1);
        model = mat4f_scale(model, ctrans->scale.x, ctrans->scale.y, ctrans->scale.z);
        model = mat4f_rotate(model, ctrans->rotation.x, ctrans->rotation.y, ctrans->rotation.z);
        model = mat4f_translate(model, ctrans->position.x, ctrans->position.y, ctrans->position.z);      

        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.m);        
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, projection.m);
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.m);


        // light source position
        glUniform3f(light_source_position_loc,
                    light_source_position.x,
                    light_source_position.y,
                    light_source_position.z);
        
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