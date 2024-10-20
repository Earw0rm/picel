#include "mesh.h"
#include <glad/glad.h>
#include <stddef.h>


#define CUBE_VERTICES_NUM (8 * 8)
#define CUBE_INDICES_NUM  (12 * 3)


static float cube_vertices[CUBE_VERTICES_NUM] = {
    // positions        // texture coordinates      // normals
     0.5f,  0.5f,  0.5f,     1.0f, 1.0f,             0.33f,  0.67f,  0.67f, 
    -0.5f,  0.5f, -0.5f,     0.0f, 1.0f,            -0.33f,  0.67f, -0.67f, 
    -0.5f,  0.5f,  0.5f,     0.0f, 0.0f,            -0.82f,  0.41f,  0.41f, 
     0.5f, -0.5f, -0.5f,     1.0f, 0.0f,             0.33f, -0.67f, -0.67f, 
    -0.5f, -0.5f, -0.5f,     0.0f, 0.0f,            -0.82f, -0.41f, -0.41f, 
     0.5f,  0.5f, -0.5f,     1.0f, 1.0f,             0.82f,  0.41f, -0.41f, 
     0.5f, -0.5f,  0.5f,     1.0f, 0.0f,             0.82f, -0.41f,  0.41f, 
    -0.5f, -0.5f,  0.5f,     0.0f, 1.0f,            -0.33f, -0.67f,  0.67f
};

GLuint cube_indices[CUBE_INDICES_NUM] ={
    0, 1, 2,
    1, 3, 4,
    5, 6, 3,
    7, 3, 6,
    2, 4, 7,
    0, 7, 6,
    0, 5, 1,
    1, 5, 3,
    5, 0, 6, 
    7, 4, 3,
    2, 1, 4,
    0, 2, 7 
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