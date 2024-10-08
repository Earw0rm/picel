#ifndef _MESH_H_
#define _MESH_H_
#include <glad/glad.h>


typedef struct mesh{
    GLuint vbo;
    GLuint vao;
    GLuint ebo;

    uint64_t vertices_num;
    float* vertices;

    uint64_t indexes_num;
    GLuint* indexes;
} mesh;

enum mesh_status{
    MESH_STATUS_OK                         =  0,
    MESH_STATUS_MAX_VALUE
};

uint8_t mesh_init(const char* filepath, mesh* m);
void mesh_destroy(mesh* m);


#endif 