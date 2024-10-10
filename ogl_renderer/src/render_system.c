#include "render_system.h"
#include "containers/darray.h"
#include "picel_math.h"


void render_system_render(ecs ecs, window w, camera main_camera){
    matrix4f view = camera_get_view(main_camera);
    float aspect_ratio = win_get_aspect_ratio(w);
    matrix4f projection = mat4f_projection(20.0f, aspect_ratio, 1, 100);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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

        glActiveTexture(GL_TEXTURE0); // Specifies which texture unit to make active.
        glBindTexture(GL_TEXTURE_2D, cmat->texture_obj); // today support only one texture


        GLint model_loc      = glGetUniformLocation(cmat->shader_program, "model");
        GLint view_loc       = glGetUniformLocation(cmat->shader_program, "view");
        GLint projection_loc = glGetUniformLocation(cmat->shader_program, "projection");    
        GLint g_sampler_loc  = glGetUniformLocation(cmat->shader_program, "g_sampler");
    

        matrix4f model = mat4f_id(1);
        model = mat4f_scale(model, ctrans->scale.x, ctrans->scale.y, ctrans->scale.z);
        model = mat4f_rotate(model, ctrans->rotation.x, ctrans->rotation.y, ctrans->rotation.z);
        model = mat4f_translate(model, ctrans->position.x, ctrans->position.y, ctrans->position.z);      

        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.m);        
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, projection.m);
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.m);
        glUniform1i(g_sampler_loc, 0); // 0 index of texture unit, mean that we say that this samples uses GL_TEXTURE0
        
        glDrawElements(GL_TRIANGLES, cmesh->vectex_count, GL_UNSIGNED_INT, 0 /*last parameter deprecated*/);

        glBindVertexArray(0);
        glUseProgram(0);

        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    } 
}