
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include "StringReader.h"
#include <errno.h>
#include <string.h>
#include <limits.h>
#include "OBJLoader.h"
#include "baseFuncs.h"

#define VAL 100000
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLfloat near = 0.01f;

struct Vertex vertices[VAL];
struct TexCoord uvs[VAL];
struct Normal normals[VAL];
int vertNum = 0, uvNum = 0, normNum = 0;
struct Vertex patrick_vertices[VAL];
struct TexCoord patrick_uvs[VAL];
struct Normal patrick_normals[VAL];
int patrick_vertNum = 0, patrick_uvNum = 0, patrick_normNum = 0;
struct Vertex spongebob_vertices[VAL];
struct TexCoord spongebob_uvs[VAL];
struct Normal spongebob_normals[VAL];
int spongebob_vertNum = 0, spongebob_uvNum = 0, spongebob_normNum = 0;
struct Vertex bootmobil_vertices[VAL];
struct TexCoord bootmobil_uvs[VAL];
struct Normal bootmobil_normals[VAL];
int bootmobil_vertNum = 0, bootmobil_uvNum = 0, bootmobil_normNum = 0;


GLfloat transMat[16];
GLfloat viewMat[16];
GLfloat projMat[16];
GLfloat patrick_transMat[16];
GLfloat patrick_viewMat[16];
GLfloat patrick_projMat[16];
GLfloat spongebob_transMat[16];
GLfloat spongebob_viewMat[16];
GLfloat spongebob_projMat[16];
GLfloat bootmobil_transMat[16];
GLfloat bootmobil_viewMat[16];
GLfloat bootmobil_projMat[16];
GLuint program, patrick_program, spongebob_program, bootmobil_program;
GLuint sbProgram;
GLuint vao;
GLuint pineapple, patrick, spongebob, bootmobil;
GLuint skyboxVAO , patrickVAO, spongebobVAO, bootmobilVAO;
unsigned int cubemapTexture;
GLfloat dir = 1;
GLfloat xScale = 0.1f;
GLfloat rotAngle = 0;
GLfloat aspect = 800/600.0f;
GLfloat * n;

GLfloat eye[] = {
    0.0f,
    3.0f,
    10.0f
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
GLfloat patrick_translate[] = {
    5.0f,
    0.0f,
    5.0f
    
};
GLfloat spongebob_translate[] = {
    8.0f,
    -2.0f,
    2.0f
    
};
GLfloat bootmobil_translate[] = {
    10.0f,
    0.0f,
    2.0f
    
};
GLfloat scaler[] = {
    0.0f,
    1.5f,
    1.0f
};


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
        printf("%s",infoLog);
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
        printf("%s",infoLog);
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
        printf("%s",infoLog);
    }
    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if(!status) {
        printf("Error linking program:");
        GLchar infoLog[1024];
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        printf("%s",infoLog);
    }
    //patrick program
    const char *patrick_fragmentText = readFile("patrickFS.glsl");
    GLuint patrick_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(patrick_fragmentShader, 1, &patrick_fragmentText, NULL);
    glCompileShader(patrick_fragmentShader);
    
    glGetShaderiv(patrick_fragmentShader, GL_COMPILE_STATUS, &status);
    if(!status) {
        printf("Error compiling fragment shader:");
        GLchar infoLog[1024];
        glGetShaderInfoLog(patrick_fragmentShader, 1024, NULL, infoLog);
        printf("%s",infoLog);
    }
    patrick_program = glCreateProgram();
    glAttachShader(patrick_program, vertexShader);
    glAttachShader(patrick_program, patrick_fragmentShader);
    glLinkProgram(patrick_program);
    glGetProgramiv(patrick_program, GL_LINK_STATUS, &status);
    if(!status) {
        printf("Error linking patrick program:");
        GLchar infoLog[1024];
        glGetProgramInfoLog(patrick_program, 1024, NULL, infoLog);
        printf("%s",infoLog);
    }
    glValidateProgram(patrick_program);
    glGetProgramiv(patrick_program, GL_VALIDATE_STATUS, &status);
    if(!status) {
        printf("Error linking patrick program:");
        GLchar infoLog[1024];
        glGetProgramInfoLog(patrick_program, 1024, NULL, infoLog);
        printf("%s",infoLog);
    }
    //spongebob program
    const char *spongebob_fragmentText = readFile("spongebobFS.glsl");
    GLuint spongebob_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(spongebob_fragmentShader, 1, &spongebob_fragmentText, NULL);
    glCompileShader(spongebob_fragmentShader);
    
    glGetShaderiv(spongebob_fragmentShader, GL_COMPILE_STATUS, &status);
    if(!status) {
        printf("Error compiling fragment shader:");
        GLchar infoLog[1024];
        glGetShaderInfoLog(spongebob_fragmentShader, 1024, NULL, infoLog);
        printf("%s",infoLog);
    }
    spongebob_program = glCreateProgram();
    glAttachShader(spongebob_program, vertexShader);
    glAttachShader(spongebob_program, spongebob_fragmentShader);
    glLinkProgram(spongebob_program);
    glGetProgramiv(spongebob_program, GL_LINK_STATUS, &status);
    if(!status) {
        printf("Error linking patrick program:");
        GLchar infoLog[1024];
        glGetProgramInfoLog(spongebob_program, 1024, NULL, infoLog);
        printf("%s",infoLog);
    }
    glValidateProgram(spongebob_program);
    glGetProgramiv(spongebob_program, GL_VALIDATE_STATUS, &status);
    if(!status) {
        printf("Error linking patrick program:");
        GLchar infoLog[1024];
        glGetProgramInfoLog(spongebob_program, 1024, NULL, infoLog);
        printf("%s",infoLog);
    }
    //bootmobil
    bootmobil_program = glCreateProgram();
    glAttachShader(bootmobil_program, vertexShader);
    glAttachShader(bootmobil_program, fragmentShader);
    glLinkProgram(bootmobil_program);
    glGetProgramiv(bootmobil_program, GL_LINK_STATUS, &status);
    if(!status) {
        printf("Error linking patrick program:");
        GLchar infoLog[1024];
        glGetProgramInfoLog(bootmobil_program, 1024, NULL, infoLog);
        printf("%s",infoLog);
    }
    glValidateProgram(bootmobil_program);
    glGetProgramiv(bootmobil_program, GL_VALIDATE_STATUS, &status);
    if(!status) {
        printf("Error linking patrick program:");
        GLchar infoLog[1024];
        glGetProgramInfoLog(bootmobil_program, 1024, NULL, infoLog);
        printf("%s",infoLog);
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
        printf("%s",infoLog);
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
        printf("%s",infoLog);
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
        printf("%s",infoLog);
    }
    glValidateProgram(sbProgram);
    glGetProgramiv(sbProgram, GL_VALIDATE_STATUS, &statussb);
    if(!statussb) {
        printf("Error linking program:");
        GLchar infoLog[1024];
        glGetProgramInfoLog(sbProgram, 1024, NULL, infoLog);
        printf("%s",infoLog);
    }


   
    
    int res = loadOBJ("spongebobs_pineapple_house.obj", vertices, uvs, normals, &vertNum, &uvNum, &normNum);
    if(res!=0)
        perror("failed to load pineapple");
    
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
    //patrick 

    int patrick_Model = loadOBJ("patrick.obj", patrick_vertices, patrick_uvs, patrick_normals, &patrick_vertNum, &patrick_uvNum, &patrick_normNum);
    if(patrick_Model !=0)
        perror("failed to load patrick");
    
    
    GLuint patrickVBO;
    glGenBuffers(1, &patrickVBO);
    glBindBuffer(GL_ARRAY_BUFFER, patrickVBO);
    glBufferData(GL_ARRAY_BUFFER, patrick_vertNum * sizeof(struct Vertex), patrick_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    GLuint patrickNormals;
    glGenBuffers(1, &patrickNormals);
    glBindBuffer(GL_ARRAY_BUFFER, patrickNormals);
    glBufferData(GL_ARRAY_BUFFER,   patrick_normNum * sizeof(struct Normal), patrick_normals, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint patrickuv;
    glGenBuffers(1, &patrickuv);
    glBindBuffer(GL_ARRAY_BUFFER, patrickuv);
    glBufferData(GL_ARRAY_BUFFER,   patrick_uvNum * sizeof(struct TexCoord), patrick_uvs, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //patrick texutre
    
    glGenTextures(1,&patrick);
    glBindTexture(GL_TEXTURE_2D, patrick);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    int patrick_width, patrick_height, patrick_channels;
    unsigned char *data = stbi_load("patrick_d.png", &patrick_width, &patrick_height, &patrick_channels,0);
    glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGB,
                patrick_width,
                patrick_height,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data
            );
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
    stbi_image_free(data);

    // create vertex array object
    glGenVertexArrays(1, &patrickVAO);
    glBindVertexArray(patrickVAO);
    glBindBuffer(GL_ARRAY_BUFFER, patrickVBO);
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
    glBindBuffer(GL_ARRAY_BUFFER, patrickNormals);
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
    glBindBuffer(GL_ARRAY_BUFFER, patrickuv);
    glVertexAttribPointer(
        2, 
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        0
    );
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //Spongebob
    int spongebob_Model = loadOBJ("spongebob_squarepants.obj", spongebob_vertices, spongebob_uvs, spongebob_normals, &spongebob_vertNum, &spongebob_uvNum, &spongebob_normNum);
    if(spongebob_Model !=0)
        perror("failed to load spongebob");
    
    
    GLuint spongebobVBO;
    glGenBuffers(1, &spongebobVBO);
    glBindBuffer(GL_ARRAY_BUFFER, spongebobVBO);
    glBufferData(GL_ARRAY_BUFFER, spongebob_vertNum * sizeof(struct Vertex), spongebob_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    GLuint spongebobNormals;
    glGenBuffers(1, &spongebobNormals);
    glBindBuffer(GL_ARRAY_BUFFER, spongebobNormals);
    glBufferData(GL_ARRAY_BUFFER,   spongebob_normNum * sizeof(struct Normal), spongebob_normals, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint spongebobuv;
    glGenBuffers(1, &spongebobuv);
    glBindBuffer(GL_ARRAY_BUFFER, spongebobuv);
    glBufferData(GL_ARRAY_BUFFER,   spongebob_uvNum * sizeof(struct TexCoord), spongebob_uvs, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //spongebobtexutre
    
    glGenTextures(1,&spongebob);
    glBindTexture(GL_TEXTURE_2D, spongebob);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    int spongebob_width, spongebob_height, spongebob_channels;
    
    data = stbi_load("spongebob.png", &spongebob_width, &spongebob_height, &spongebob_channels,0);
    stbi_set_flip_vertically_on_load(1);
    glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGBA,
                spongebob_width,
                spongebob_height,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data
            );
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
    stbi_image_free(data);

    // create vertex array object
    glGenVertexArrays(1, &spongebobVAO);
    glBindVertexArray(spongebobVAO);
    glBindBuffer(GL_ARRAY_BUFFER, spongebobVBO);
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
    glBindBuffer(GL_ARRAY_BUFFER, spongebobNormals);
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
    glBindBuffer(GL_ARRAY_BUFFER, spongebobuv);
    glVertexAttribPointer(
        2, 
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        0
    );
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //bootmobil

    int bootmobil_Model = loadOBJ("Patty Wagon(OBJ).obj", bootmobil_vertices, bootmobil_uvs, bootmobil_normals, &bootmobil_vertNum, &bootmobil_uvNum, &bootmobil_normNum);
    if(bootmobil_Model !=0)
        perror("failed to load bootmobil");
    
    
    GLuint bootmobilVBO;
    glGenBuffers(1, &bootmobilVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bootmobilVBO);
    glBufferData(GL_ARRAY_BUFFER, bootmobil_vertNum * sizeof(struct Vertex), bootmobil_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
    GLuint bootmobilNormals;
    glGenBuffers(1, &bootmobilNormals);
    glBindBuffer(GL_ARRAY_BUFFER, bootmobilNormals);
    glBufferData(GL_ARRAY_BUFFER,   bootmobil_normNum * sizeof(struct Normal), bootmobil_normals, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint bootmobiluv;
    glGenBuffers(1, &bootmobiluv);
    glBindBuffer(GL_ARRAY_BUFFER, bootmobiluv);
    glBufferData(GL_ARRAY_BUFFER,   bootmobil_uvNum * sizeof(struct TexCoord), bootmobil_uvs, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //bootmobil texutre
    
    
    

    // create vertex array object
    glGenVertexArrays(1, &bootmobilVAO);
    glBindVertexArray(bootmobilVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bootmobilVBO);
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
    glBindBuffer(GL_ARRAY_BUFFER, bootmobilNormals);
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
    glBindBuffer(GL_ARRAY_BUFFER, bootmobiluv);
    glVertexAttribPointer(
        2, 
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        0
    );
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    //skybox

    GLfloat skyBoxVertices[] = {

       
        -1.0f,1.0f,1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
        1.0f,1.0f,1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,1.0f, 1.0f,0.0f,
        -1.0f,1.0f,-1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f, 1.0f, 0.0f,
        1.0f,1.0f,-1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, -1.0f, 0.0f, 0.0f
    };

    unsigned int skyBoxIndices[] = {
       
        0, 2, 3, 0, 3, 1,
        2, 6, 7, 2, 7, 3,
        6, 4, 5, 6, 5, 7,
        4, 0, 1, 4, 1, 5,
        0, 4, 6, 0, 6, 2,
        1, 5, 7,1, 7, 3
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

void draw(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    
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
    
    
    // reihenfolge scale -> rotate -> translate !!!!!
    
    
    
    
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
    glUniform1f(mshineLocation, 32.0f);
    GLint laLocation = glGetUniformLocation(program, "light.ambient");
    GLint ldLocation = glGetUniformLocation(program, "light.diffuse");
    GLint lsLocation = glGetUniformLocation(program, "light.specular");
    
    glUniform3f(laLocation, 0.5f,0.0f,0.0f);
    glUniform3f(ldLocation, 0.2f,0.0f,0.0f);
    glUniform3f(lsLocation, 1.0f,1.0f,1.0f);
    
    glBindVertexArray(vao);
    
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDrawArrays(GL_TRIANGLES, 0,vertNum);
    glBindVertexArray(0);
    
//spongebob

    glUseProgram(spongebob_program);
    identity(spongebob_transMat);
    identity(spongebob_viewMat);
    lookAt(spongebob_viewMat, eye, look, up);
    perspective(spongebob_projMat,90, aspect, 0.001f, 1000.0f);
    rotatex(spongebob_transMat, spongebob_transMat, 4.5);
    translate(spongebob_transMat,spongebob_transMat,spongebob_translate); 
    GLint spongebob_cameraLocation = glGetUniformLocation(spongebob_program, "camera");
    glUniform3f(spongebob_cameraLocation,x,y,z);
    GLint spongebob_colorLocation = glGetUniformLocation(spongebob_program, "color");
    glUniform3f(spongebob_colorLocation, 1.0f, 0.24f, 0.02f);
    GLint spongebob_ModelLoc = glGetUniformLocation(spongebob_program, "model");
    glUniformMatrix4fv(spongebob_ModelLoc,1,GL_FALSE, spongebob_transMat);
    GLint spongebob_viewLoc = glGetUniformLocation(spongebob_program, "view");
    glUniformMatrix4fv(spongebob_viewLoc,1, GL_FALSE, spongebob_viewMat);
    GLint spongebob_projLoc = glGetUniformLocation(spongebob_program, "projection");
    glUniformMatrix4fv(spongebob_projLoc,1, GL_FALSE, spongebob_projMat);
    GLint spongebob_maLocation = glGetUniformLocation(spongebob_program, "material.ambient");
    GLint spongebob_mdLocation = glGetUniformLocation(spongebob_program, "material.diffuse");
    GLint spongebob_msLocation = glGetUniformLocation(spongebob_program, "material.specular");
    GLint spongebob_mshineLocation = glGetUniformLocation(spongebob_program, "material.shininess");
    glUniform3f(spongebob_maLocation, 0.2f, 0.2f, 0.2f);
    glUniform3f(spongebob_mdLocation, 0.8f, 0.8f, 0.8f);
    glUniform3f(spongebob_msLocation, 0.0f, 0.0f, 0.0f);
    glUniform1f(spongebob_mshineLocation, 64.0f);
    GLint spongebob_laLocation = glGetUniformLocation(spongebob_program, "light.ambient");
    GLint spongebob_ldLocation = glGetUniformLocation(spongebob_program, "light.diffuse");
    GLint spongebob_lsLocation = glGetUniformLocation(spongebob_program, "light.specular");
    
    glUniform3f(spongebob_laLocation, 0.5f,0.0f,0.0f);
    glUniform3f(spongebob_ldLocation, 0.2f,0.0f,0.0f);
    glUniform3f(spongebob_lsLocation, 1.0f,1.0f,1.0f);

    glActiveTexture(GL_TEXTURE13);
    glBindTexture(GL_TEXTURE_2D, spongebob);
    GLuint spongebob_Loc = glGetUniformLocation(spongebob_program, "ourTexture");
    glUniform1i(spongebob_Loc,13);
    glBindVertexArray(spongebobVAO);

    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glCullFace(GL_CULL_FACE);
    glDrawArrays(GL_TRIANGLES, 0,spongebob_vertNum);

    //patrick
    glUseProgram(patrick_program);
    identity(patrick_transMat);
    identity(patrick_viewMat);
    lookAt(patrick_viewMat, eye, look, up);
    perspective(patrick_projMat,90, aspect, 0.1f, 1000.0f);
    //rotatey(patrick_transMat, patrick_transMat, rotAngle);
    translate(patrick_transMat,patrick_transMat,patrick_translate); 
    GLint patrick_cameraLocation = glGetUniformLocation(patrick_program, "camera");
    glUniform3f(patrick_cameraLocation,x,y,z);
    GLint patrick_colorLocation = glGetUniformLocation(patrick_program, "color");
    glUniform3f(patrick_colorLocation, 1.0f, 0.24f, 0.02f);
    GLint patrick_ModelLoc = glGetUniformLocation(patrick_program, "model");
    glUniformMatrix4fv(patrick_ModelLoc,1,GL_FALSE, patrick_transMat);
    GLint patrick_viewLoc = glGetUniformLocation(patrick_program, "view");
    glUniformMatrix4fv(patrick_viewLoc,1, GL_FALSE, patrick_viewMat);
    GLint patrick_projLoc = glGetUniformLocation(patrick_program, "projection");
    glUniformMatrix4fv(patrick_projLoc,1, GL_FALSE, patrick_projMat);
    GLint patrick_maLocation = glGetUniformLocation(patrick_program, "material.ambient");
    GLint patrick_mdLocation = glGetUniformLocation(patrick_program, "material.diffuse");
    GLint patrick_msLocation = glGetUniformLocation(patrick_program, "material.specular");
    GLint patrick_mshineLocation = glGetUniformLocation(patrick_program, "material.shininess");
    glUniform3f(patrick_maLocation, 0.2f, 0.2f, 0.2f);
    glUniform3f(patrick_mdLocation, 0.8f, 0.8f, 0.8f);
    glUniform3f(patrick_msLocation, 0.0f, 0.0f, 0.0f);
    glUniform1f(patrick_mshineLocation, 64.0f);
    GLint patrick_laLocation = glGetUniformLocation(patrick_program, "light.ambient");
    GLint patrick_ldLocation = glGetUniformLocation(patrick_program, "light.diffuse");
    GLint patrick_lsLocation = glGetUniformLocation(patrick_program, "light.specular");
    
    glUniform3f(patrick_laLocation, 0.5f,0.0f,0.0f);
    glUniform3f(patrick_ldLocation, 0.2f,0.0f,0.0f);
    glUniform3f(patrick_lsLocation, 1.0f,1.0f,1.0f);

    glActiveTexture(GL_TEXTURE12);
    glBindTexture(GL_TEXTURE_2D, patrick);
    GLuint patrick_Loc = glGetUniformLocation(patrick_program, "ourTexture");
    glUniform1i(patrick_Loc,12);
    glBindVertexArray(patrickVAO);

    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glDrawArrays(GL_TRIANGLES, 0,patrick_vertNum);
    glDisable(GL_CULL_FACE);
    //bootmobil
    glUseProgram(bootmobil_program);
    identity(bootmobil_transMat);
    
    identity(bootmobil_viewMat);

    lookAt(bootmobil_viewMat, eye, look, up);
    perspective(bootmobil_projMat,90, aspect, 0.1f, 1000.0f);
    rotatey(bootmobil_transMat, bootmobil_transMat, rotAngle);
    translate(bootmobil_transMat,bootmobil_transMat,bootmobil_translate); 
    GLint bootmobil_cameraLocation = glGetUniformLocation(bootmobil_program, "camera");
    glUniform3f(bootmobil_cameraLocation,x,y,z);
    GLint bootmobil_colorLocation = glGetUniformLocation(bootmobil_program, "color");
    glUniform3f(bootmobil_colorLocation, 1.0f, 0.24f, 0.02f);
    GLint bootmobil_ModelLoc = glGetUniformLocation(bootmobil_program, "model");
    glUniformMatrix4fv(bootmobil_ModelLoc,1,GL_FALSE, bootmobil_transMat);
    GLint bootmobil_viewLoc = glGetUniformLocation(bootmobil_program, "view");
    glUniformMatrix4fv(bootmobil_viewLoc,1, GL_FALSE, bootmobil_viewMat);
    GLint bootmobil_projLoc = glGetUniformLocation(bootmobil_program, "projection");
    glUniformMatrix4fv(bootmobil_projLoc,1, GL_FALSE, bootmobil_projMat);
    GLint bootmobil_maLocation = glGetUniformLocation(bootmobil_program, "material.ambient");
    GLint bootmobil_mdLocation = glGetUniformLocation(bootmobil_program, "material.diffuse");
    GLint bootmobil_msLocation = glGetUniformLocation(bootmobil_program, "material.specular");
    GLint bootmobil_mshineLocation = glGetUniformLocation(bootmobil_program, "material.shininess");
    glUniform3f(bootmobil_maLocation, 0.2f, 0.2f, 0.2f);
    glUniform3f(bootmobil_mdLocation, 0.8f, 0.8f, 0.8f);
    glUniform3f(bootmobil_msLocation, 0.0f, 0.0f, 0.0f);
    glUniform1f(bootmobil_mshineLocation, 64.0f);
    GLint bootmobil_laLocation = glGetUniformLocation(bootmobil_program, "light.ambient");
    GLint bootmobil_ldLocation = glGetUniformLocation(bootmobil_program, "light.diffuse");
    GLint bootmobil_lsLocation = glGetUniformLocation(bootmobil_program, "light.specular");
    
    glUniform3f(bootmobil_laLocation, 0.5f,0.0f,0.0f);
    glUniform3f(bootmobil_ldLocation, 0.2f,0.0f,0.0f);
    glUniform3f(bootmobil_lsLocation, 1.0f,1.0f,1.0f);

    
    glBindVertexArray(bootmobilVAO);

    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glDrawArrays(GL_TRIANGLES, 0,bootmobil_vertNum);
    
     if(rotAngle <360) {
        rotAngle += 0.01f;
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
    else if (key == GLFW_KEY_UP && action == GLFW_REPEAT)
        patrick_translate[1] = patrick_translate[1] +0.5f;
    else if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
        patrick_translate[1] = patrick_translate[1] -0.5f;  
    else if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT)
        patrick_translate[2] = patrick_translate[2] +0.5f; 
        
    else if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT)
        patrick_translate[2] = patrick_translate[2] -0.5f; 
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