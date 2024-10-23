#ifndef _MESH_2H_
#define _MESH_2H_

#include "picel_math.h"
#include <glad/glad.h>
#include "containers/darray.h"

#include <assimp/cimport.h>     // Plain-C interface
#include <assimp/scene.h>       // output data structures
#include <assimp/postprocess.h> // post processing flag (ie aiProcess_Triangulate)

struct mesh_impl;
typedef struct mesh_impl* mesh;


GLint mesh_vao(mesh m);
void mesh_to_gpu(mesh mesh);
mesh mesh_from_assimp(struct aiMesh* aimesh, const struct aiScene* scene, const char* workdir);
uint64_t mesh_vertices_len(mesh mesh);
uint64_t mesh_indices_len(mesh mesh);
uint64_t mesh_textures_len(mesh mesh);
void mesh_activate_textures(mesh mesh);

#endif 