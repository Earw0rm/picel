#include "mesh.h"
#include <glad/glad.h>

static size_t number_of_meshes_used    = 0;
static size_t number_of_meshes_created = 0;

struct mesh_data{
    GLint vbo;
    GLint vao;
    GLint ebo;

    uint64_t vertices_num;
    float vertices*;

    uint64_t indexes_num;
    uint64_t indexes*;
};

mesh createMesh(const char* filepath){
    return nullptr;
}
void destroyMesh(mesh mesh){
    //TODO implement this
}