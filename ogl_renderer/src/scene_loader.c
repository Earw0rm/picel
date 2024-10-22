#include "amesh.h"
#include "picel_math.h"
#include "darray.h"
#include "shader.h"
#include <glad/glad.h>
#include <assimp/cimport.h>     // Plain-C interface
#include <assimp/scene.h>       // output data structures
#include <assimp/postprocess.h> // post processing flag (ie aiProcess_Triangulate)

typedef enum {
    TEXTURE_TYPE_DIFFUSE  = 0, 
    TEXTURE_TYPE_SPECULAR = 1,


    TEXTURE_TYPE_MAX
} TEXTURE_TYPE;

static char* texture_type2name[TEXTURE_TYPE_MAX] = {
    [TEXTURE_TYPE_DIFFUSE]  "material.texture_specular_n", //26
    [TEXTURE_TYPE_SPECULAR] "material.texture_diffuse__n",
};

static inline char* ttype2name(TEXTURE_TYPE type, uint8_t texture_num){
    char* name = texture_type2name[type];
    name[26] = (char)('0' + texture_num);         
    return name;
}

typedef struct vertex{
    vector3f position;
    vector2f texture_coords;
    vector3f normal;
} vertex;

typedef struct mesh{
    darray vertices;
    darray indices;
    darray textures;
    GLint vao, ebo, vbo;
} mesh;

typedef struct texture {
    uint32_t id;
    TEXTURE_TYPE type;
} texture;

typedef struct scene{
    darray meshes;
    char* path;
} scene;


typedef struct model{
    darray meshes;
    char* dirname;
} model;


void mesh_setup(mesh mesh){
    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    
    glBufferData(GL_ARRAY_BUFFER, 
                 darray_lenght(mesh.vertices) * sizeof(vertex),
                 darray_at(mesh.vertices, 0), 
                 GL_STATIC_DRAW);    

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ARRAY_BUFFER, 
                 darray_lenght(mesh.indices) * sizeof(GLuint),
                 darray_at(mesh.indices, 0), 
                 GL_STATIC_DRAW);    

    // vertex positions 
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);             

    // texture_coords 
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), offsetof(vertex, texture_coords));

    // normals 
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), offsetof(vertex, normal));

    glBindVertexArray(0);
}

void mesh_draw(mesh mesh, shader sh){
    glUseProgram(sh.program); // if it is not used yet


    uint8_t diffuse_texture_num  = 0;
    uint8_t specular_texture_num = 0;

    for(uint8_t i = 0; i < darray_lenght(mesh.textures), ++i){
        glActiveTexture(GL_TEXTURE0 + i);
        texture* txt = (texture*)darray_at(mesh.textures, i);
        char* full_name = ttype2name(txt->type, i);

        // uniform sampler2D texture_specular1;
        GLint loc = glGetUniformLocation(sp->program, full_name);
        glUniform1i(loc, i);

        glBindTexture(GL_TEXTURE_2D, txt->id);
    }
    glActiveTexture(GL_TEXTURE0);

    //draw
    glBindVertexArray(mesh.vao);
    glDrawElements(GL_TRIANGLES, darray_lenght(mesh.indices), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void model_draw(shader sh, model m){
    for(int i = 0; i < darray_lenght(m.meshes), ++i){
        mesh* m = (mesh*) darray_at(m.meshes, i);
        mesh_draw(*m, sh);
    }
}

static inline 
mesh process_ai_mesh(struct aiMesh* aimesh, const struct aiScene* scene){
    darray vertices = darray_alloc(vertex);
    darray indices  = darray_alloc(GLuint);
    darray textures = darray_alloc(texture);

    mesh mmesh;
    mmesh.ebo = 0;
    mmesh.vao = 0;
    mmesh.ebo = 0;
    mmesh.vertices = vertices;
    mmesh.indices  = indices;
    mmesh.textures = textures;


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
        // load diffuse and specular textures
        // A material object internally stores an array of 
        // texture locations for each texture type.

        // we need to check that texture not already loaded into our memory
        // mean that for different mesh they can use same texture
        // so if this texture already loaded - skip it 



        // vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
        // {
        //     vector<Texture> textures;
        //     for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        //     {
        //         aiString str;
        //         mat->GetTexture(type, i, &str);
        //         Texture texture;
        //         texture.id = TextureFromFile(str.C_Str(), directory);
        //         texture.type = typeName;
        //         texture.path = str;
        //         textures.push_back(texture);
        //     }
        //     return textures;
        // }


    }


    return mmesh;
}

static inline 
void process_ai_node(struct aiNode* node, const struct aiScene* scene, model* m){

    for(uint32_t i = 0; i < node->mNumMeshes; ++i){
        struct aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
        darray_push_back(m->meshes, process_ai_mesh(aimesh, scene));
    }

    for(uint32_t i = 0; i < node->mNumChildren; ++i){
        process_ai_node(node->mChildren[i], scene, m);
    }
}


amesh amesh_load(const char* filepath){
    model m;
    m.meshes = darray_alloc(mesh);
    m.dirname = "asd"

    const struct aiScene* scene;
    if((scene = aiImportFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs)) == nullptr 
        || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE 
        || scene->mRootNode == nullptr){
        LOG_FATAL("cannot import scene using asimp: %s", aiGetErrorString());
        return nullptr;
    }

    //process scene start hire 
    process_ai_node(scene->mRootNode, scene, &m);
    //process scene end hire 
    aiReleaseImport(scene);
    return mesh;
}

