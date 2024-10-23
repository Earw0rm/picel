#include "picel_math.h"

#include "shader.h"
#include "mesh2.h"
#include "scene.h"

#include <glad/glad.h>
#include <assimp/cimport.h>     // Plain-C interface
#include <assimp/scene.h>       // output data structures
#include <assimp/postprocess.h> // post processing flag (ie aiProcess_Triangulate)


struct scene_impl{
    darray meshes;
    const char* path;
    char* workdir; //
};

size_t scene_sizeof(void){
    return sizeof(struct scene_impl);
}

darray scene_get_meshes(scene s){
    return s->meshes;
}

// renderer end


static inline 
void process_ai_node(struct aiNode* node, const struct aiScene* ais, scene internal_scene){
    
    for(uint32_t i = 0; i < node->mNumMeshes; ++i){
        struct aiMesh* aimesh = ais->mMeshes[node->mMeshes[i]];
        darray_push_back(internal_scene->meshes, mesh_from_assimp(aimesh, ais, internal_scene->workdir));
    }

    for(uint32_t i = 0; i < node->mNumChildren; ++i){
        process_ai_node(node->mChildren[i], ais, internal_scene);
    }
}

static char* 
dir_from_path(const char* path) {
    const char* last_slash = strrchr(path, '/');  
    if(last_slash != nullptr){
        size_t length = last_slash - path + 1;  
        char* directory = malloc(length + 1);   
        if (directory != nullptr) {
            strncpy(directory, path, length);   
            directory[length] = '\0';           
        }
        return directory;
    }
    
    return nullptr;
}

scene scene_load(const char* filepath){
    scene internal_scene = malloc(sizeof(struct scene_impl));
    internal_scene->meshes = darray_alloc_fix(mesh_sizeof());
    internal_scene->path = filepath;
    internal_scene->workdir = dir_from_path(filepath);

    const struct aiScene* scene;
    if((scene = aiImportFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs)) == nullptr 
        || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE 
        || scene->mRootNode == nullptr){
        LOG_FATAL("cannot import scene using asimp: %s", aiGetErrorString());
        return nullptr;
    }

    //process scene start hire 
    process_ai_node(scene->mRootNode, scene, internal_scene);


    //process scene end hire 
    aiReleaseImport(scene);
    return internal_scene;
}

