#ifndef _MESH_2H_
#define _MESH_2H_

#include "picel_math.h"
#include <glad/glad.h>
#include "darray.h"

#include <assimp/cimport.h>     // Plain-C interface
#include <assimp/scene.h>       // output data structures
#include <assimp/postprocess.h> // post processing flag (ie aiProcess_Triangulate)

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

void mesh_to_gpu(mesh mesh);
mesh mesh_from_assimp(struct aiMesh* aimesh, const struct aiScene* scene);

#endif 