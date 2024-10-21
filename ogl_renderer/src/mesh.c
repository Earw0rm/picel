#include "mesh.h"
#include <glad/glad.h>
#include <stddef.h>


#define CUBE_VERTICES_NUM (24 * 8)
#define CUBE_INDICES_NUM  (12 * 3)


static float cube_vertices[CUBE_VERTICES_NUM] = {
    // positions          // texture coordinates      // normals
     1.0f,  1.0f, -1.0f,   0.0f, 1.0f,                0.0f,  1.0f,  0.0f,  // Вершина 1
    -1.0f,  1.0f, -1.0f,   1.0f, 1.0f,                0.0f,  1.0f,  0.0f,  // Вершина 5
    -1.0f,  1.0f,  1.0f,   1.0f, 0.0f,                0.0f,  1.0f,  0.0f,  // Вершина 7
     1.0f,  1.0f,  1.0f,   0.0f, 0.0f,                0.0f,  1.0f,  0.0f,  // Вершина 3

     1.0f, -1.0f,  1.0f,   0.0f, 1.0f,                0.0f,  0.0f,  1.0f,  // Вершина 4
     1.0f,  1.0f,  1.0f,   1.0f, 1.0f,                0.0f,  0.0f,  1.0f,  // Вершина 3
    -1.0f,  1.0f,  1.0f,   1.0f, 0.0f,                0.0f,  0.0f,  1.0f,  // Вершина 7
    -1.0f, -1.0f,  1.0f,   0.0f, 0.0f,                0.0f,  0.0f,  1.0f,  // Вершина 8

    -1.0f, -1.0f,  1.0f,   0.0f, 1.0f,               -1.0f,  0.0f,  0.0f,  // Вершина 8
    -1.0f,  1.0f,  1.0f,   1.0f, 1.0f,               -1.0f,  0.0f,  0.0f,  // Вершина 7
    -1.0f,  1.0f, -1.0f,   1.0f, 0.0f,               -1.0f,  0.0f,  0.0f,  // Вершина 5
    -1.0f, -1.0f, -1.0f,   0.0f, 0.0f,               -1.0f,  0.0f,  0.0f,  // Вершина 6

    -1.0f, -1.0f, -1.0f,   0.0f, 1.0f,                0.0f, -1.0f,  0.0f,  // Вершина 6
     1.0f, -1.0f, -1.0f,   1.0f, 1.0f,                0.0f, -1.0f,  0.0f,  // Вершина 2
     1.0f, -1.0f,  1.0f,   1.0f, 0.0f,                0.0f, -1.0f,  0.0f,  // Вершина 4
    -1.0f, -1.0f,  1.0f,   0.0f, 0.0f,                0.0f, -1.0f,  0.0f,  // Вершина 8

     1.0f, -1.0f, -1.0f,   0.0f, 1.0f,                1.0f,  0.0f,  0.0f,  // Вершина 2
     1.0f,  1.0f, -1.0f,   1.0f, 1.0f,                1.0f,  0.0f,  0.0f,  // Вершина 1
     1.0f,  1.0f,  1.0f,   1.0f, 0.0f,                1.0f,  0.0f,  0.0f,  // Вершина 3
     1.0f, -1.0f,  1.0f,   0.0f, 0.0f,                1.0f,  0.0f,  0.0f,  // Вершина 4

    -1.0f,  1.0f, -1.0f,   0.0f, 1.0f,                0.0f,  0.0f, -1.0f,  // Вершина 5
     1.0f,  1.0f, -1.0f,   1.0f, 1.0f,                0.0f,  0.0f, -1.0f,  // Вершина 1
     1.0f, -1.0f, -1.0f,   1.0f, 0.0f,                0.0f,  0.0f, -1.0f,  // Вершина 2
    -1.0f, -1.0f, -1.0f,   0.0f, 0.0f,                0.0f,  0.0f, -1.0f   // Вершина 6
};

GLuint cube_indices[CUBE_INDICES_NUM] = {
    // Front face
    0, 1, 2,  2, 3, 0,
    // Back face
    4, 5, 6,  6, 7, 4,
    // Left face
    8, 9, 10,  10, 11, 8,
    // Right face
    12, 13, 14,  14, 15, 12,
    // Top face
    16, 17, 18,  18, 19, 16,
    // Bottom face
    20, 21, 22,  22, 23, 20
};



// TODO implement reading from disk
// TODO now all meshes are on the gpu all the time. Refactor
uint8_t mesh_init(const char* filepath, mesh* m){
    uint8_t mesh_status = MESH_STATUS_OK;

    glGenVertexArrays(1, &m->vao);
    glBindVertexArray(m->vao);


    
    glGenBuffers(1, &m->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);    
    // load data from memory to GPU
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, CUBE_INDICES_NUM * sizeof(GLuint), cube_indices, GL_STATIC_DRAW);

    glGenBuffers(1, &m->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m->vbo);    
    // load data from memory to GPU
    glBufferData(GL_ARRAY_BUFFER, CUBE_VERTICES_NUM * sizeof(float), cube_vertices, GL_STATIC_DRAW);


    // glVertexAttribPointer call work relative last binded buffer
    glVertexAttribPointer( /*lenght = 3 (3 vertises). Stride => vertices + textures*/
            /*layout (location = 0)*/ 0,
            /*how many data for vertices*/3,
            GL_FLOAT,
            GL_FALSE,
            8 * sizeof(float), // stride
            /*offset inside VBO*/(void*)0
    );

    // texture vertex attribute pointer
    glVertexAttribPointer( 
            /*layout (location = 1)*/ 1,
            /*how many data for textures*/2, 
            GL_FLOAT,
            GL_FALSE,
            8 * sizeof(float),
            /*offset inside VBO*/(void*)((3 * sizeof(float)))
    );

    // texture vertex attribute pointer
    glVertexAttribPointer( 
            /*layout (location = 1)*/ 2,
            /*how many data for normals*/3, 
            GL_FLOAT,
            GL_FALSE,
            8 * sizeof(float),
            /*offset inside VBO*/(void*)((5 * sizeof(float)))
    );




    m->vertices_num = CUBE_VERTICES_NUM ;
    m->vertices = cube_vertices;
    m->indexes_num = CUBE_INDICES_NUM;
    m->indexes = cube_indices;

    return mesh_status;
}



void mesh_destroy(mesh* m){
    glDeleteBuffers(1, &m->vbo);
    glDeleteBuffers(1, &m->vao);
    glDeleteBuffers(1, &m->ebo);        
}