#ifndef OBJLOADER_H
#define OBJLOADER_H

struct Vertex {
    float x, y, z;
    
};

struct TexCoord {
    float u, v;
};

struct Normal {
    float x, y, z;
};


int loadOBJ(const char * filename,
    struct Vertex  *out_vertices,
    struct TexCoord *out_uvs,
    struct Normal *out_normals,
    int *vertNum, int* uvNum, int* normNum);

#endif