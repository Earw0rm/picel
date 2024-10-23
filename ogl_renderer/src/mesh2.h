#ifndef _MESH_2H_
#define _MESH_2H_

#include "picel_math.h"
#include <glad/glad.h>
#include "containers/darray.h"
#include "shader.h"

#include <assimp/cimport.h>     // Plain-C interface
#include <assimp/scene.h>       // output data structures
#include <assimp/postprocess.h> // post processing flag (ie aiProcess_Triangulate)

typedef struct mesh_impl* mesh2;


GLuint mesh_vao(mesh2 m);
void mesh_to_gpu(mesh2 mesh);
mesh2 mesh_from_assimp(struct aiMesh* aimesh, const struct aiScene* scene, const char* workdir);
uint64_t mesh_vertices_len(mesh2 mesh);
uint64_t mesh_indices_len(mesh2 mesh);
uint64_t mesh_textures_len(mesh2 mesh);
void mesh_activate_textures(mesh2 mesh, shader sh);

// runtime only
size_t mesh_sizeof(void);


#endif 