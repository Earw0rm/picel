#include "mesh2.h"


/**
 * load mesh into gpu. Fill vbo/ebo/vao.
 */
void mesh_to_gpu(mesh mesh){
    if(mesh.vao != 0 
      || mesh.vbo != 0 
      || mesh.ebo != 0){
        LOG_WARN("mesh_to_gpu calling twise. VAO : %i, VBO : %i, EBO : %i");
    }

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

static inline 
mesh mesh_from_assimp(struct aiMesh* aimesh, const struct aiScene* scene){
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