#include "mesh2.h"
#include "texture2.h"


typedef struct vertex{
    vector3f position;
    vector2f texture_coords;
    vector3f normal;
} vertex;


struct mesh_impl{
    darray vertices;
    darray indices;
    darray textures;
    GLuint vao, ebo, vbo;
    bool is_initialized;
};

GLuint mesh_vao(mesh2 m){
    return m->vao;
}

void mesh_activate_textures(mesh2 mesh, shader sh){
    // we want to do something
    uint8_t diffuse_texture_num  = 0;
    uint8_t specular_texture_num = 0;

    for(uint8_t i = 0; i < darray_lenght(mesh->textures); ++i){
        texture2 *t = (texture2 *)darray_at(mesh->textures, i);
        if(texture_is_diffuse(*t)){
            texture_activate(*t, diffuse_texture_num++, sh);
        }else if(texture_is_specular(*t)){
            texture_activate(*t, specular_texture_num++, sh);
        }
    }
}

size_t mesh_sizeof(void){
    return sizeof(struct mesh_impl);
}

uint64_t mesh_vertices_len(mesh2 mesh){
    return darray_lenght(mesh->vertices);
}
uint64_t mesh_indices_len(mesh2 mesh){
    return darray_lenght(mesh->indices);
}
uint64_t mesh_textures_len(mesh2 mesh){
    return darray_lenght(mesh->textures);
}

void mesh_active(mesh2 mesh, shader sp){
    if(mesh->is_initialized) {
        mesh_to_gpu(mesh);
    }
    
    glBindVertexArray(mesh->vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    mesh_activate_textures(mesh, sp);    
}

/**
 * load mesh into gpu. Fill vbo/ebo/vao.
 * If mesh has textures, load them into gpu too.
 */
void mesh_to_gpu(mesh2 mesh){
    if(mesh->is_initialized) {
        return;
    }

    glGenVertexArrays(1, &mesh->vao);
    glGenBuffers(1, &mesh->vbo);
    glGenBuffers(1, &mesh->ebo);

    glBindVertexArray(mesh->vao);
 
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);   
    glBufferData(GL_ARRAY_BUFFER, 
                 darray_lenght(mesh->vertices) * sizeof(vertex),
                 darray_at(mesh->vertices, 0), 
                 GL_STATIC_DRAW);    

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ARRAY_BUFFER, 
                 darray_lenght(mesh->indices) * sizeof(GLuint),
                 darray_at(mesh->indices, 0), 
                 GL_STATIC_DRAW);    

    // vertex positions 
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);             

    // texture_coords 
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, texture_coords));

    // normals 
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));

    for(uint8_t i = 0; i < darray_lenght(mesh->textures); ++i){
        texture2* t = (texture2*) darray_at(mesh->textures, i);
        texture_to_gpu(*t);
    }

    glBindVertexArray(0);   
    mesh->is_initialized = true;
}


mesh2 mesh_from_assimp(struct aiMesh* aimesh, const struct aiScene* scene, const char* workdir){
    darray vertices = darray_alloc(vertex);
    darray indices  = darray_alloc(GLuint);
    darray textures = darray_alloc(texture2);

    mesh2 mmesh = malloc(sizeof(struct mesh_impl));
    mmesh->ebo = 0;
    mmesh->vao = 0;
    mmesh->ebo = 0;
    mmesh->vertices = vertices;
    mmesh->indices  = indices;
    mmesh->textures = textures;


    for(uint32_t i = 0; i < aimesh->mNumVertices; ++i){
        vertex v;
        v.position.x = aimesh->mVertices[i].x;
        v.position.y = aimesh->mVertices[i].y;
        v.position.z = aimesh->mVertices[i].z; 

        v.normal.x = aimesh->mNormals[i].x;
        v.normal.y = aimesh->mNormals[i].y;
        v.normal.z = aimesh->mNormals[i].z; 

        if(aimesh->mTextureCoords[0] != nullptr){
            v.texture_coords.x = aimesh->mTextureCoords[0][i].x;
            v.texture_coords.y = aimesh->mTextureCoords[0][i].y;            
        }else{
            v.texture_coords.x = 0;
            v.texture_coords.y = 0;
        }

        darray_push_back(vertices, v);
    }    

    // Assimp's interface defines each mesh as having an array of faces,
    // where each face represents a single primitive, which in our case 
    // (due to the aiProcess_Triangulate option) are always triangles. 
    // A face contains the indices of the vertices we need to draw in what
    //  order for its primitive. So if we iterate over all the faces and 
    //  store all the face's indices in the indices vector we're all set:
    for(uint32_t i = 0; i < aimesh->mNumFaces; ++i){
        struct aiFace face = aimesh->mFaces[i];
        for(uint8_t j = 0; j < face.mNumIndices; ++j){
            darray_push_back(indices, face.mIndices[j]);
        }
    }

    
    if(aimesh->mMaterialIndex >= 0){
        struct aiMaterial* mat = scene->mMaterials[aimesh->mMaterialIndex];
        darray diffuse_textures = textures_from_assimp(mat, aiTextureType_DIFFUSE, TEXTURE_TYPE_DIFFUSE, workdir);
        darray specular_textures = textures_from_assimp(mat, aiTextureType_SPECULAR, TEXTURE_TYPE_SPECULAR, workdir);        

        for(uint8_t i = 0; i < darray_lenght(diffuse_textures); ++i){
            texture2* t = (texture2*) darray_at(diffuse_textures, i);
            darray_push_back(textures, *t);
        }

        for(uint8_t i = 0; i < darray_lenght(specular_textures); ++i){
            texture2* t = (texture2*) darray_at(specular_textures, i);
            darray_push_back(textures, *t);
        }

        darray_free(diffuse_textures);
        darray_free(specular_textures);
    }


    return mmesh;
}