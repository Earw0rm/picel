#include "texture2.h"
#include "stb_image.h"
#include <assimp/cimport.h>     // Plain-C interface
#include <assimp/scene.h>       // output data structures
#include <assimp/postprocess.h> // post processing flag (ie aiProcess_Triangulate)
#include <string.h>

static char* texture_type2name[TEXTURE_TYPE_MAX] = {
    [TEXTURE_TYPE_DIFFUSE]  "mat.texture_specular_n", //21
    [TEXTURE_TYPE_SPECULAR] "mat.texture_diffuse__n",
};

static inline char* 
ttype2name(TEXTURE_TYPE type, uint8_t texture_num){
    char* name = texture_type2name[type];
    name[21] = (char)('0' + texture_num);         
    return name;
}

static inline unsigned char* 
texture_read(const char* work_dir, const char* name){
    char* concat = malloc(strlen(work_dir) + strlen(name) + 4);
    strcat(concat, work_dir);
    strcat(concat, name);


    // because opengl y axis is flip
    stbi_set_flip_vertically_on_load(1);
    int width = 0, height = 0, /* bits per pixel */ bpp = 0;

    unsigned char* img_data = stbi_load(concat, &width, &height, &bpp, 0);
    free(concat);
    return img_data;
}


/**
 * @return darray<texture>
 */
darray textures_from_assimp(struct aiMaterial *mat, aiTextureType type, TEXTURE_TYPE internal_type, const char* workdir){
    darray textures = darray_alloc(texture);
    uint32_t text_count = aiGetMaterialTextureCount(mat, type);
    for(uint32_t i = 0; i < text_count; ++i){
        struct aiString path; 
        aiReturn ret = aiGetMaterialTexture(mat, type, i, &path,
                       nullptr, nullptr,nullptr,nullptr,nullptr, nullptr);
        if(ret != aiReturn_SUCCESS){
            LOG_ERROR("aiGetMaterialTexture does not success");
            darray_free(textures);
            return nullptr;
        }

        //load texture.../

    }
    return textures;
}
void loadMaterialTextures(struct aiMaterial *mat, struct aiTextureType type, char* typeName){
    aiGetMaterialTexture(mat, type, )    
}
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