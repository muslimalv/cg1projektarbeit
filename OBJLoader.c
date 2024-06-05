#include "OBJLoader.h"
#include <stdio.h>
#include <string.h>
#define VAL 60000

int loadOBJ(
    const char * filename,
    struct Vertex  *out_vertices,
    struct TexCoord *out_uvs,
    struct Normal *out_normals,
    int *vertNum, int* uvNum, int* normNum) {
    
    unsigned int vertexIndices[VAL], uvIndices[VAL], normalIndices[VAL];
    struct Vertex  temp_vertices[VAL];
    struct TexCoord temp_uvs[VAL];
    struct Normal temp_normals[VAL];
    int vCount = 0, nCount = 0, uvCount = 0;
    int temp_Vertex_Index = 0, temp_uv_Index =0, temp_Normal_Index =0;
    FILE * file = fopen(filename, "r");
    if( file == NULL ){
        printf("Impossible to open the file !\n");
        return 1;
    }   
    printf("hallo");
    
    while(1) {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if(res == EOF)
            break;
        if ( strcmp( lineHeader, "vn" ) == 0 ){
            struct Normal normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals[temp_Normal_Index] = normal;
            temp_Normal_Index ++;
        }else if ( strcmp( lineHeader, "vt" ) == 0 ){
            struct TexCoord uv;
            fscanf(file, "%f %f\n", &uv.u, &uv.v );
            temp_uvs[temp_uv_Index] = uv;
            temp_uv_Index ++;
        }else if(strcmp(lineHeader, "v") == 0) {
            struct Vertex vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices[temp_Vertex_Index] = vertex;
            temp_Vertex_Index ++;
        } else if ( strcmp( lineHeader, "f" ) == 0 ){
            //std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9){
                printf("File can't be read by  simple parser\n");
                return 1;
            }
            vertexIndices[vCount] = vertexIndex[0];
            vertexIndices[vCount+1] = vertexIndex[1];
            vertexIndices[vCount+2] = vertexIndex[2];
            vCount = vCount + 3;
            uvIndices[uvCount]  =  (uvIndex[0]);
            uvIndices[uvCount+1]  = (uvIndex[1]);
            uvIndices[uvCount+2]  =  (uvIndex[2]);
            uvCount = uvCount +3;
            normalIndices[nCount]=(normalIndex[0]);
            normalIndices[nCount+1]=(normalIndex[1]);
            normalIndices[nCount+2]=(normalIndex[2]);
            nCount = nCount +3;
        }
    }
    
    for (unsigned int i = 0; i < vCount; i++) {
        unsigned int vertexIndex = vertexIndices[i];
        struct Vertex vertex = temp_vertices[vertexIndex-1];
        out_vertices[*vertNum] = (vertex);
        *vertNum +=1;
    }
    for (unsigned int i = 0; i < nCount; i++) {
        unsigned int normalIndex = normalIndices[i];
        struct Normal normal = temp_normals[normalIndex-1];
        out_normals[*normNum] = normal;
        *normNum +=1;
    }
    for (unsigned int i = 0; i < uvCount; i++) {
        unsigned int uvIndex = uvIndices[i];
        struct TexCoord uv = temp_uvs[uvIndex-1];
        out_uvs[*uvNum] = uv;
        *uvNum +=1;
    }
    return 0;
}