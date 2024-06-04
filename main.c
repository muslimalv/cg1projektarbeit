
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include "StringReader.h"
#include <errno.h>
#include <string.h>
#include <limits.h>
#define PI		3.14159265358979323846
#define VAL 60000
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Vertex {
    float x, y, z;
    
};

struct TexCoord {
    float u, v;
};

struct Normal {
    float x, y, z;
};
int pineapple;
GLfloat near = 0.01f;

struct Vertex vertices[VAL];
struct TexCoord uvs[VAL];
struct Normal normals[VAL];
int vertNum = 0, uvNum = 0, normNum = 0;
int count[3];
GLuint program;
GLuint sbProgram;
GLuint vao;
GLuint skyboxVAO;
unsigned int cubemapTexture;
GLfloat dir = 1;
GLfloat xScale = 0.1f;
GLfloat rotAngle = 0;
GLfloat aspect = 800/600.0f;
GLfloat * n;

GLfloat eye[] = {
    0.0f,
    0.0f,
    0.0f
};
GLfloat up[] = {
    0.0f,
    1.0f,
    0.0f
};

GLfloat look[] = {
    0.0f,
    0.0f,
    0.0f  //1000
};

GLfloat input[] = {
    1.0f,
    1.0f,
    1.0f,
    1.0f
};
GLfloat testvektor[] = {
    0.0f,
    0.0f,
    1.0f
    
};
GLfloat scaler[] = {
    0.0f,
    1.5f,
    1.0f
};
float cAdd;
//obj loader
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
    int Vtemp = 0, uvtemp =0, Ntemp =0;
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
            temp_normals[Ntemp] = normal;
            Ntemp ++;
        }else if ( strcmp( lineHeader, "vt" ) == 0 ){
            struct TexCoord uv;
            fscanf(file, "%f %f\n", &uv.u, &uv.v );
            temp_uvs[uvtemp] = uv;
            uvtemp ++;
        }else if(strcmp(lineHeader, "v") == 0) {
            struct Vertex vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices[Vtemp] = vertex;
            Vtemp ++;
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
    return 0;
}

void init(void) {
    // create and compile vertex shader
    const char* vertexText = readFile("vertexShader.glsl");
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    glShaderSource(vertexShader, 1, &vertexText, NULL);
    glCompileShader(vertexShader);
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if(!status) {
        printf("Error compiling vertex shader:");
        GLchar infoLog[1024];
        glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
        printf(infoLog);
    }

    // create and compile vertex shader
    const char *fragmentText = readFile("fragmentShader.glsl");
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentText, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if(!status) {
        printf("Error compiling fragment shader:");
        GLchar infoLog[1024];
        glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
        printf(infoLog);
    }
    // create and link shader program
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(!status) {
        printf("Error linking program:");
        GLchar infoLog[1024];
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        printf(infoLog);
    }
    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if(!status) {
        printf("Error linking program:");
        GLchar infoLog[1024];
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        printf(infoLog);
    }

    //skybox shader
    // create and compile vertex shader
    const char* vertexTextsb = readFile("skyboxVS.glsl");
    GLuint vertexShadersb = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShadersb, 1, &vertexTextsb, NULL);
    glCompileShader(vertexShadersb);
    GLint statussb;
    glGetShaderiv(vertexShadersb, GL_COMPILE_STATUS, &statussb);
    if(!statussb) {
        printf("Error compiling vertex shader:");
        GLchar infoLog[1024];
        glGetShaderInfoLog(vertexShadersb, 1024, NULL, infoLog);
        printf(infoLog);
    }

    // create and compile vertex shader
    const char *fragmentTextsb = readFile("skyboxFS.glsl");
    GLuint fragmentShadersb = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShadersb, 1, &fragmentTextsb, NULL);
    glCompileShader(fragmentShadersb);
    
    glGetShaderiv(fragmentShadersb, GL_COMPILE_STATUS, &statussb);
    if(!statussb) {
        printf("Error compiling fragment shader:");
        GLchar infoLog[1024];
        glGetShaderInfoLog(fragmentShadersb, 1024, NULL, infoLog);
        printf(infoLog);
    }
    // create and link shader program
    sbProgram = glCreateProgram();
    glAttachShader(sbProgram, vertexShadersb);
    glAttachShader(sbProgram, fragmentShadersb);
    glLinkProgram(sbProgram);
    glGetProgramiv(sbProgram, GL_LINK_STATUS, &statussb);
    if(!statussb) {
        printf("Error linking program skybox:");
        GLchar infoLog[1024];
        glGetProgramInfoLog(sbProgram, 1024, NULL, infoLog);
        printf(infoLog);
    }
    glValidateProgram(sbProgram);
    glGetProgramiv(sbProgram, GL_VALIDATE_STATUS, &statussb);
    if(!statussb) {
        printf("Error linking program:");
        GLchar infoLog[1024];
        glGetProgramInfoLog(sbProgram, 1024, NULL, infoLog);
        printf(infoLog);
    }


   
    
    int res = loadOBJ("spongebobs_pineapple_house.obj", vertices, uvs, normals, &vertNum, &uvNum, &normNum);
    GLfloat norms[3*normNum];
    int count = 0;
    /*for (struct Normal n : normals)
    {
        norms[count] = n.x;
        norms[count+1] = n.y;
        norms[count+2] = n.z;
        count = count +3;
    }
    n = norms;*/
    
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertNum * sizeof(struct Vertex), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    GLuint norm;
    glGenBuffers(1, &norm);
    glBindBuffer(GL_ARRAY_BUFFER, norm);
    glBufferData(GL_ARRAY_BUFFER,   normNum * sizeof(struct Normal), normals, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // create vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    

    // create texture object
    /*glGenTextures(1,&pineapple);
    glBindTexture(GL_TEXTURE_2D, pineapple);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    int width, height, channels;
    unsigned char *data = stbi_load("", &width, &height, &channels);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
    stbi_image_free(data);*/

    glVertexAttribPointer(
        0, 
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        0
    );
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, norm);
    glVertexAttribPointer(
        1, 
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        0
    );
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //skybox

    GLfloat skyBoxVertices[] = {

        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f,  1.0f, 0.0f,
        1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 1.0f, 0.0f,0.0f,
        -1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,  0.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,  1.0f, 1.0f
    };

    unsigned int skyBoxIndices[] = {
        //right
        1,2,6,
        6,5,1,
        //left
        0,4,7,
        7,3,0,
        //top
        4,5,6,
        6,7,4,
        //bottom
        0,3,2,
        2,1,0,
        //back
        0,1,5,
        5,4,0,
        //front
        3,7,6,
        6,2,3
    };
    unsigned int skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1,  &skyboxEBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyBoxVertices), skyBoxVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyBoxIndices), skyBoxIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5* sizeof(float), (void*)0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5* sizeof(float), (GLvoid*) (3*sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    
    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    for (int i = 0; i < 6; i++) {
        int width, height, channels;
        unsigned char* data = stbi_load("sky.png", &width, &height, &channels, 0);
        if(data) {
            stbi_set_flip_vertically_on_load(1);
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGBA,
                width,
                height,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                data
            );
            stbi_image_free(data);
        } else {
            perror("fehler beim laden vom bild");
            stbi_image_free(data);
        }

    }
    
    

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glViewport(0, 0, 800, 600);
   
}

void identity(GLfloat* out) {
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if(i == j) {
                out[i*4+j] = 1.0f;
            } else {
                out[i*4+j] = 0.0f;
            }
        }
        
    }
    
    
}

void translate(GLfloat* out, GLfloat* in, GLfloat* v) {
    /*GLfloat T[] = {
        1.0f, 0.0f, 0.0f, v[0],
        0.0f, 1.0f, 0.0f, v[1],
        0.0f, 0.0f, 1.0f, v[2],
        0.0f, 0.0f, 0.0f, 1.0f
    };*/
    GLfloat T[] = {
        1.0f,0.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,0.0f,
        0.0f,0.0f,1.0f,0.0f,
        v[0],v[1],v[2],1.0f
    };
    /*for (int i = 0; i < 4; i++) {
        for ( int j = 0; j < 4; j++) {
            out[j + i * 4] = 0;

            for (int k = 0; k < 4; k++) {
                out[j + i * 4] += in[k + i * 4] * T[j + k * 4];
            }
        }
    }*/
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
             out[i*4+j] = T[i*4] * in[j*4] +  T[i*4+1] * in[j*4+1] + T[i*4+2] * in[j*4+2] + T[i*4+3] * in[j*4+3];
             
        }
        
        //out[i] = T[i*4] * in[0] +  T[i*4+1] * in[1] + T[i*4+2] * in[2] + T[i*4+3] * in[3];
    }
}

void scale(GLfloat* out, GLfloat* in, GLfloat* v) {
    GLfloat S[] = {
        v[0], 0.0f, 0.0f, 0.0f,
        0.0f, v[1], 0.0f, 0.0f,
        0.0f, 0.0f, v[2], 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            out[i*4+j] =S[i*4] * in[j*4] +  S[i*4+1] * in[j*4+1] + S[i*4+2] * in[j*4+2] + S[i*4+3] * in[j*4+3];
        }
        
        //out[i] = S[i*4] * in[0] +  S[i*4+1] * in[1] + S[i*4+2] * in[2] + S[i*4+3] * in[3];
        
    }
}
void rotatez(GLfloat* out, GLfloat* in, GLfloat angle) {
    /*GLfloat R[] = {
        cos(angle), -sin(angle), 0.0f, 0.0f,
        sin(angle), cos(angle), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };*/
    GLfloat R[] = {
        cos(angle), sin(angle), 0.0f, 0.0f,
        -sin(angle), cos(angle), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
             out[i*4+j] = R[i*4] * in[j*4] +  R[i*4+1] * in[j*4+1] + R[i*4+2] * in[j*4+2] + R[i*4+3] * in[j*4+3];
        }
        //out[i] = R[i*4] * in[0] +  R[i*4+1] * in[1] + R[i*4+2] * in[2] + R[i*4+3] * in[3];
        
    }
}

void rotatex(GLfloat* out, GLfloat* in, GLfloat angle) {
    /*GLfloat R[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cos(angle), -sin(angle), 0.0f,
        0.0f, sin(angle), cos(angle), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };*/
    GLfloat R[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cos(angle), sin(angle), 0.0f,
        0.0f, -sin(angle), cos(angle), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
             out[i*4+j] = R[i*4] * in[j*4] +  R[i*4+1] * in[j*4+1] + R[i*4+2] * in[j*4+2] + R[i*4+3] * in[j*4+3];
        }
        //out[i] = R[i*4] * in[0] +  R[i*4+1] * in[1] + R[i*4+2] * in[2] + R[i*4+3] * in[3];
        
    }
}

void rotatey(GLfloat* out, GLfloat* in, GLfloat angle) {
    /*GLfloat R[] = {
        cos(angle), 0.0f, sin(angle), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -sin(angle), 0.0f, cos(angle), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };*/
    GLfloat R[] = {
        cos(angle), 0.0f, -sin(angle), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        sin(angle), 0.0f, cos(angle), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
             out[i*4+j] = R[i*4] * in[j*4] +  R[i*4+1] * in[j*4+1] + R[i*4+2] * in[j*4+2] + R[i*4+3] * in[j*4+3];
        }
        //out[i] = R[i*4] * in[0] +  R[i*4+1] * in[1] + R[i*4+2] * in[2] + R[i*4+3] * in[3];
        
    }
}

void normalize(GLfloat* out, GLfloat* in) {
    GLfloat x = 1/(sqrt((in[0]*in[0]) + (in[1]*in[1]) + (in[2]*in[2])));

    for (int i = 0; i < 3; i++)
    {
        out[i] = x * in[i];
    }
    
}

void cross(GLfloat* out, GLfloat* a, GLfloat* b) {
    out[0] = a[1]*b[2] - a[2]*b[1];
    out[1] = a[2]*b[0] - a[0]*b[2];
    out[2] = a[0]*b[1] - a[1]*b[0];
}

void lookAt(GLfloat* out, GLfloat* eye, GLfloat* center, GLfloat* up) {
    GLfloat n[3];
    GLfloat u[3];
    GLfloat v[3];

    //n = Eye - center(look)
    for (int i = 0; i < 3; i++)
    {
        n[i] = eye[i] - center[i];
    }

    //u = Up x n
    cross(u, up, n);

    //v = n x u
    cross(v, n, u);

    // normalize vectors
    GLfloat nNorm[3];
    GLfloat uNorm[3];
    GLfloat vNorm[3];
    normalize(nNorm, n);
    normalize(uNorm, u);
    normalize(vNorm, v);

    // translation
    GLfloat t[] = {
        -(uNorm[0] * eye[0] + uNorm[1] * eye[1] + uNorm[2] * eye[2]),
        -(vNorm[0] * eye[0] + vNorm[1] * eye[1] + vNorm[2] * eye[2]),
        -(nNorm[0] * eye[0] + nNorm[1] * eye[1] + nNorm[2] * eye[2])
    };
    

    //Drehmatrix
    /*out[0] = uNorm[0]; out[1] = uNorm[1]; out[2] = uNorm[2]; out[3] = t[0];
    out[4] = vNorm[0]; out[5] = vNorm[1]; out[6] = vNorm[2]; out[7] = t[1];
    out[8] = nNorm[0]; out[9] = nNorm[1]; out[10] = nNorm[2]; out[11] = t[2];
    out[12] = 0.0f; out[13] = 0.0f; out[14] = 0.0f; out[15] = 1.0f;*/
    out[0] = uNorm[0]; out[1] = vNorm[0]; out[2] = nNorm[0]; out[3] = 0.0f;
    out[4] = uNorm[1]; out[5] = vNorm[1]; out[6] = nNorm[1]; out[7] = 0.0f;
    out[8] = uNorm[2]; out[9] = vNorm[2]; out[10] = nNorm[2]; out[11] = 0.0f;
    out[12] = t[0]; out[13] = t[1]; out[14] = t[2]; out[15] = 1.0f;
}

void perspective(GLfloat* out, GLfloat fovy, GLfloat aspect, GLfloat near, GLfloat far) {
    GLfloat fovyRadians = fovy * PI/180.0f;
    float t = tan(fovyRadians/2) *near ;
    float b = -t;
    float r = aspect *t;
    float l = -r;

    identity(out);
    out[0] = 2/(r -l); 
    out[5] = 2/(t-b); 
    out[8] =  (1/near) * ((r +l)/(r-l));
    out[9] =  (1/near) * ((t+b)/(t-b));
    out[10] = - (1/near) * ((far + near)/(far - near));
    out[11] = - 1/near;
    out[14] = - 2*far/(far - near); out[15] = 0.0f;
    /*out[0] = 2*near/(r-l);
    out[5] = 2*near/(t-b);
    out[8] = (r+l) / (r-l);
    out[9] = (t+b) /(t-b);
    out[10] =-(far+near)/(far-near);
    out[11] = -1.0f;
    out[14] = -2*far*near /(far-near);
    out[15] =0.0f;*/
    
}

void draw(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    GLfloat transMat[16];
    GLfloat viewMat[16];
    GLfloat projMat[16];
    identity(transMat);
    identity(viewMat);
    lookAt(viewMat, eye, look, up);
    perspective(projMat,90, aspect, 0.1f, 1000.0f); 
    //draw skybox
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    glUseProgram(sbProgram);
    //glUniformMatrix4fv(glGetUniformLocation(sbProgram, "view"),1, GL_FALSE,viewMat);
    //glUniformMatrix4fv(glGetUniformLocation(sbProgram, "projection"),1, GL_FALSE,projMat);
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    GLuint sbLoc = glGetUniformLocation(sbProgram, "skybox");
    glUniform1i(sbLoc,0);
    glDrawElements(GL_TRIANGLES,36, GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);

    glUseProgram(program);
    
    //identity(projMat);
    // reihenfolge scale -> rotate -> translate !!!!!
    
    //scale(transMat, transMat, scaler);
     //rotatez(transMat, transMat, rotAngle);
    //rotatex(transMat, transMat, rotAngle);
    //rotatey(transMat,transMat,rotAngle);
    //rotatey(viewMat, viewMat, rotAngle);
    //translate(transMat,transMat,testvektor);  
    
    
    float x = eye[0];
    float y = eye[1];
    float z = eye[2];
    GLint cameraLocation = glGetUniformLocation(program, "camera");
    glUniform3f(cameraLocation,x,y,z);
    GLint colorLocation = glGetUniformLocation(program, "color");
    glUniform3f(colorLocation, 1.0f, 0.24f, 0.02f);
    GLint ModelLoc = glGetUniformLocation(program, "model");
    glUniformMatrix4fv(ModelLoc,1,GL_FALSE, transMat);
    GLint viewLoc = glGetUniformLocation(program, "view");
    glUniformMatrix4fv(viewLoc,1, GL_FALSE, viewMat);
    GLint projLoc = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(projLoc,1, GL_FALSE, projMat);
    GLint maLocation = glGetUniformLocation(program, "material.ambient");
    GLint mdLocation = glGetUniformLocation(program, "material.diffuse");
    GLint msLocation = glGetUniformLocation(program, "material.specular");
    GLint mshineLocation = glGetUniformLocation(program, "material.shininess");
    glUniform3f(maLocation, 1.0f, 0.5f, 0.31f);
    glUniform3f(mdLocation, 1.0f, 0.5f, 0.31f);
    glUniform3f(msLocation, 0.5f, 0.5f, 0.5f);
    glUniform1f(mshineLocation, 64.0f);
    GLint laLocation = glGetUniformLocation(program, "light.ambient");
    GLint ldLocation = glGetUniformLocation(program, "light.diffuse");
    GLint lsLocation = glGetUniformLocation(program, "light.specular");
    
    glUniform3f(laLocation, 0.5f,0.0f,0.0f);
    glUniform3f(ldLocation, 0.2f,0.0f,0.0f);
    glUniform3f(lsLocation, 1.0f,1.0f,1.0f);
    
    glBindVertexArray(vao);
    
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    glDrawArrays(GL_TRIANGLES, 0,vertNum);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    if (scaler[0] >= 10.0f)
    {
        dir = -1;
    } else if (scaler[0] <= 0.0f)
    {
        dir = 1;
    }
    //eye[2] = eye[2] + 0.0001;
    
    
    //std::cout << look[2] << std::endl;
    
    
    scaler[1] = scaler[1] + dir * 0.01f;
   
    //scaler[0] = scaler[0] + dir * 0.01f;
    
    if(rotAngle < 360) {
        rotAngle += 0.1f;
        
    } else {
        rotAngle = 0;
    }
    
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W && action == GLFW_REPEAT)
        eye[0] = eye[0] -0.5f;
    else if (key == GLFW_KEY_S && action == GLFW_REPEAT)
        eye[0] = eye[0] +0.5f;
    else if (key == GLFW_KEY_A && action == GLFW_REPEAT)
        eye[2] = eye[2] +0.5f;
    else if (key == GLFW_KEY_D && action == GLFW_REPEAT)
        eye[2] = eye[2] -0.5f;
    else if (key == GLFW_KEY_X && action == GLFW_REPEAT)
        eye[1] = eye[1] +0.5f;
    else if (key == GLFW_KEY_Z && action == GLFW_REPEAT)
        eye[1] = eye[1] -0.5f;

    else if (key == GLFW_KEY_U && action == GLFW_REPEAT)
        look[0] = look[0] -0.5f;
    else if (key == GLFW_KEY_J && action == GLFW_REPEAT)
        look[0] = look[0] +0.5f;
    else if (key == GLFW_KEY_H && action == GLFW_REPEAT)
        look[2] = look[2] +0.5f;
    else if (key == GLFW_KEY_K && action == GLFW_REPEAT)
        look[2] = look[2] -0.5f;
    else if (key == GLFW_KEY_O && action == GLFW_REPEAT)
        look[1] = look[1] +0.5f;
    else if (key == GLFW_KEY_P && action == GLFW_REPEAT)
        look[1] = look[1] -0.5f;    
}
int main(void)
{
    
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
    if (!window)
    {
        printf("Failed to create Window\n");
        glfwTerminate();
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    glewInit();

    init();

    while (!glfwWindowShouldClose(window))
    {
        draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }

    glfwTerminate();
    return 0;
}