#ifndef _MESH_H_
#define _MESH_H_

typedef struct mesh_data* mesh;


mesh createMesh(const char* filepath);
void destroyMesh(mesh mesh);


#endif 