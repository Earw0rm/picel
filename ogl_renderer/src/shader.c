#include "shader.h"
#include "math.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#define INFO_BUFSZ 0x1000


static int64_t
shader_read(const char* path, const char** shader_data){
    int64_t ret;
    FILE *fp;

    if((fp = fopen(path, "r")) == nullptr){
        LOG_ERROR("cannot open file: %s \n %s", path, strerror(errno));
        ret = SHADER_STATUS_BAD_OPEN_FILE;
        return ret;
    }

    if(fseek(fp, 0, SEEK_END) != 0){
       LOG_ERROR("cannot fseek file at the end: %s \n %s", path, strerror(errno));
       ret = SHADER_STATUS_BAD_FSEEK;
       goto cleanup;
    }
    
    uint64_t sz = ftell(fp);
    if(sz == 0){
       LOG_ERROR("sizeof shader equal to 0: %s \n %s", path,  strerror(errno));
       ret = SHADER_STATUS_BAD_FTELL;
       goto cleanup;
    }

    if(fseek(fp, 0, SEEK_SET) != 0){
       LOG_ERROR("cannot fseek file at the start: %s \n %s", path, strerror(errno));
       ret = SHADER_STATUS_BAD_FSEEK;
       goto cleanup;
    }

    (*shader_data) = malloc(sizeof(char) * (sz + 1) );
    if((*shader_data) == nullptr){
       LOG_ERROR("cannot allocate data for shader: %s \n %s", path, strerror(errno));
       ret = SHADER_STATUS_BAD_ALLOC;
       goto cleanup;
    }

    size_t bytes_read = fread((void*)(*shader_data), sizeof(char), sz, fp);
    if(bytes_read != sz){
       LOG_ERROR("cannot read full file: %s. Bytes read: %u, Filesize: %u \n %s", path,
                 bytes_read, sz, strerror(errno));
       ret = SHADER_STATUS_BAD_FREAD;
       goto cleanup;        
    }


    size_t path_len = strlen(path);
    const char* shader_extension = ((const char *)(path + path_len - 4)); 
    if(!((strcmp(shader_extension, "vert") == 0) || (strcmp(shader_extension, "frag") == 0))){
       LOG_ERROR("unsoported shader type: %s", path);
       ret = SHADER_STATUS_BAD_EXTENSION;
       goto cleanup;        
    }

    ret = sz;

cleanup:
    if(fp != nullptr && fclose(fp) != 0){
        LOG_FATAL("cannot close file");
        ret = SHADER_STATUS_BAD_FCLOSE;
    }
    if(ret < 0  && (*shader_data) != nullptr){
        free((void*)(*shader_data));
        (*shader_data) = nullptr;
    }

    return ret;
}

static GLint 
create_shader_and_compile(GLenum shader_type, const int32_t shader_data_sz, const char* shader_data){
    // TODO
    //since we read for every one shader, shader_data_sz and shader_data uses crunch hire

    GLint compile_res;
    GLint shader_id = glCreateShader(shader_type);  



    glShaderSource(shader_id, 1, &shader_data, &shader_data_sz);
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_res);
    if(!compile_res){
        char buf[INFO_BUFSZ];
        glGetShaderInfoLog(shader_id, INFO_BUFSZ, nullptr, buf);
        LOG_FATAL("cannot compile shader.Type: %x, Reason: %s", shader_type, buf);

        shader_id = SHADER_STATUS_BAD_COMPILE;
    }
    return shader_id;
}



int8_t 
shader_init(const char* vert_path, const char* frag_path, bool check_location, shader* sp){
    int8_t res = SHADER_STATUS_OK;
    GLint frag_shader_id = 0;
    GLint vert_shader_id = 0;
    sp->program = 0;

    int64_t vert_shader_res;
    const char* vert_shader_data = nullptr;
    int64_t frag_shader_res;
    const char* frag_shader_data = nullptr;
    GLint link_res;
    GLint val_res;
    if((vert_shader_res = shader_read(vert_path, &vert_shader_data)) < 0){
        LOG_ERROR("cannot read vertex_shader: %i", vert_shader_res);
        res = vert_shader_res;
        goto shader_cleanup;
    }
    if((frag_shader_res = shader_read(frag_path, &frag_shader_data)) < 0){
        LOG_ERROR("cannot read fragment_shader: %i", vert_shader_res);
        res = frag_shader_res;
        goto shader_cleanup;
    }

    vert_shader_id = 
                create_shader_and_compile(GL_VERTEX_SHADER, vert_shader_res, vert_shader_data);

    if(vert_shader_id <= 0){
        res = vert_shader_id;
        goto shader_cleanup;
    }

    frag_shader_id = create_shader_and_compile(GL_FRAGMENT_SHADER, frag_shader_res, frag_shader_data);    
    if(frag_shader_id <= 0){
        res = frag_shader_id;
        goto shader_cleanup;
    }

    sp->program = glCreateProgram();
    if(sp->program == 0){
        LOG_ERROR("cannot create program: %i", sp->program);
        res = SHADER_STATUS_BAD_PROGRAM_CREATE;
        goto shader_cleanup;
    }

    glAttachShader(sp->program, vert_shader_id);
    glAttachShader(sp->program, frag_shader_id);

    glLinkProgram(sp->program);
    glGetProgramiv(sp->program, GL_LINK_STATUS, &link_res);
    if(!link_res){
        char buf[INFO_BUFSZ];
        glGetProgramInfoLog(sp->program, INFO_BUFSZ, nullptr, buf);
        LOG_FATAL("cannot link program. Reason: %s", buf);
        res = SHADER_STATUS_BAD_PROGRAM_LINKAGE;
        goto shader_cleanup;
    } 

    // this validations check that program 
    // compatable with current opengl runtime
    glValidateProgram(sp->program);
    glGetProgramiv(sp->program, GL_VALIDATE_STATUS, &val_res);
    if(!val_res){
        char buf[INFO_BUFSZ];
        glGetProgramInfoLog(sp->program, INFO_BUFSZ, nullptr, buf);
        LOG_FATAL("Validation of shader programm failed: %s", buf);
        res = SHADER_STATUS_BAD_PROGRAM_VALIDATION;
        goto shader_cleanup;
    }    


    // these 3 uniforms should be in any custom shader.
    // We don't use the location outside of this class. They are just needed for testing.
    // uniform need to be called after program link
    if(check_location){
        sp->uniform_location_model = glGetUniformLocation(sp->program, "model");
        if(sp->uniform_location_model == -1){
            LOG_FATAL("Cannot find uniform with name model. Shader must include uniform with this name");
            res = SHADER_STATUS_BAD_UNIFORM;
            goto shader_cleanup;
        }

        sp->uniform_location_projection = glGetUniformLocation(sp->program, "projection");
        if(sp->uniform_location_projection == -1){
            LOG_FATAL("Cannot find uniform with name projection. Shader must include uniform with this name");
            res = SHADER_STATUS_BAD_UNIFORM;
            goto shader_cleanup;
        }    

        sp->uniform_location_view = glGetUniformLocation(sp->program, "view");
        if(sp->uniform_location_view == -1){
            LOG_FATAL("Cannot find uniform with name view. Shader must include uniform with this name");
            res = SHADER_STATUS_BAD_UNIFORM;
            goto shader_cleanup;
        }

        sp->uniform_location_g_sampler = glGetUniformLocation(sp->program, "g_sampler");
        if(sp->uniform_location_g_sampler == -1){
            LOG_FATAL("Cannot find uniform with name g_sampler. Shader must include uniform with this name");
            res = SHADER_STATUS_BAD_UNIFORM;
            goto shader_cleanup;
        }
    }



    sp->is_initialized = true;

shader_cleanup:
    if(vert_shader_id > 0){
            glDeleteShader(vert_shader_id);
    }
    if(frag_shader_id > 0){
            glDeleteShader(frag_shader_id);
    }
    if(vert_shader_data != nullptr){
        free((void*) vert_shader_data);
    }
    if(frag_shader_data != nullptr){
        free((void*) frag_shader_data);
    }

    if(res != SHADER_STATUS_OK){
        if(sp->program != 0){
            glDeleteProgram(sp->program);
        }
    }

    return res;
}

void shader_destroy(shader* sp){
    glDeleteProgram(sp->program);
    sp->is_initialized = false;
    sp->program =  0;
    sp->uniform_location_model = -1;
    sp->uniform_location_view = -1;
    sp->uniform_location_projection = -1;    
}

