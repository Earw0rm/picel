#include "texture2.h"
#include "stb_image.h"
#include <assimp/cimport.h>     // Plain-C interface
#include <assimp/scene.h>       // output data structures
#include <assimp/postprocess.h> // post processing flag (ie aiProcess_Triangulate)
#include <string.h>
#include <glad/glad.h>
#include "shader.h"



struct texture_impl {
    uint32_t texture_obj;
    TEXTURE_TYPE type;
    unsigned char* texture_data;
    char* path; // for comparing with another textures
    int width, height, bpp;enum
    bool is_initialized;
};

bool texture_is_diffuse(texture t){
    return t->type == TEXTURE_TYPE_DIFFUSE;
}

bool texture_is_specular(texture t){
    return t->type == TEXTURE_TYPE_SPECULAR;
}

static char* texture_type2name[TEXTURE_TYPE_MAX] = {
    [TEXTURE_TYPE_DIFFUSE]  "mat.texture_specular_n", //21
    [TEXTURE_TYPE_SPECULAR] "mat.texture_diffuse__n",
};

const char* 
texture_get_name(texture tp, uint8_t cell){
    char* name = texture_type2name[tp->type];
    name[21] = (char)('0' + cell);         
    return name;
}

static inline unsigned char* 
texture_read(const char* work_dir, const char* name, int* width, int* height, int* bpp){
    // todo +1?
    char* concat = malloc(strlen(work_dir) + strlen(name) + 1);
    strcpy(concat, work_dir);
    strcat(concat, name);


    // because opengl y axis is flip
    stbi_set_flip_vertically_on_load(1);

    unsigned char* img_data = stbi_enumload(concat, width, height, bpp, 0);
    free(concat);
    return img_data;
}

size_t texture_sizeof(void){
    return sizeof(texture_impl);
}

/**
 * @return darray<texture>
 */
darray textures_from_assimp(struct aiMaterial *mat,enum aiTextureType type, TEXTURE_TYPE internal_type, const char* workdir){
    darray textures = darray_alloc(sizeof(struct texture_impl));
    uint32_t text_count = aiGetMaterialTextureCount(mat, type);
    texture emplace_texture = malloc(sizeof(struct texture_impl));

    for(uint32_t i = 0; i < text_count; ++i){
        struct aiString path; 
        aiReturn ret = aiGetMaterialTexture(mat, type, i, &path,
                       nullptr, nullptr,nullptr,nullptr,nullptr, nullptr);
        if(ret != aiReturn_SUCCESS){
            LOG_ERROR("aiGetMaterialTexture does not success");
            darray_free(textures);
            return nullptr;
        }
        int width, height, bpp;
        unsigned char* texture_data = texture_read(workdir, path.data, &width, &height, &bpp);

        if(texture_data == nullptr){
            LOG_ERROR("cannot read texture, workdir: %s, path: %s");
            darray_free(textures);
            return nullptr;
        }

        emplace_texture->path           = path;
        emplace_texture->texture_data   = texture_data;
        emplace_texture->is_initialized = false;
        emplace_texture->type           = internal_type;
        emplace_texture->width          = width;
        emplace_texture->height         = height;
        emplace_texture->bpp            = bpp;

        //after that array contain copy of the texture memory
        darray_emplace_back(textures, emplace_texture);

    }
    free(emplace_texture);    
    return textures;
}

void texture_to_gpu(texture tp){
    if(tp->is_initialized) return;
    glGenTextures(1, &tp->texture_obj);
    glBindTexture(GL_TEXTURE_2D, tp->texture_obj);
    // define texture image and load data to GPU
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA, /* how we store inside gpu*/
        tp->width,
        tp->height,
        0,
        GL_RGBA, /*format of stb image*/ 
        GL_UNSIGNED_BYTE,
        tp->texture_data
    );

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    tp->is_initialized = true;
}

void texture_activate(texture tp, uint8_t cell, shader sp,){
    if(!tp->is_initialized){
        texture_to_gpu(tp);
    }

    glActiveTexture(GL_TEXTURE0 + cell);
    const char* full_name = texture_get_name(tp, cell);

    // uniform sampler2D texture_specular1;
    GLint loc = glGetUniformLocation(sp->program, full_name);
    glUniform1i(loc, cell);
    glBindTexture(GL_TEXTURE_2D, tp->texture_obj);
}

void texture_destroy(texture tp){
    if(tp->texture_data != nullptr){
        stbi_image_free(tp->texture_data);
    }
    glDeleteTextures(1, &tp->texture_obj);
}