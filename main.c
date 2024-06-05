
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
#define PI		3.14159265358979323846
#define VAL 60000
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
int pineapple;
GLfloat near = 0.01f;

struct Vertex vertices[VAL];
struct TexCoord uvs[VAL];
struct Normal normals[VAL];
int vertNum = 0, uvNum = 0, normNum = 0;
struct Vertex patrick_vertices[VAL];
struct TexCoord patrick_uvs[VAL];
struct Normal patrick_normals[VAL];
int patrick_vertNum = 0, patrick_uvNum = 0, patrick_normNum = 0;


GLfloat transMat[16];
GLfloat viewMat[16];
GLfloat projMat[16];
GLfloat patrick_transMat[16];
GLfloat patrick_viewMat[16];
GLfloat patrick_projMat[16];
GLuint program, patrick_program;
GLuint sbProgram;
GLuint vao;
GLuint skyboxVAO , patrickVAO;
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
GLfloat testvektor[] = {
    5.0f,
    0.0f,
    5.0f
    
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
    patrick_program = glCreateProgram();
    glAttachShader(patrick_program, vertexShader);
    glAttachShader(patrick_program, fragmentShader);
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
    //patrick TODO1111

    int patrick = loadOBJ("patrick.obj", patrick_vertices, patrick_uvs, patrick_normals, &patrick_vertNum, &patrick_uvNum, &patrick_normNum);
    if(patrick !=0)
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
    glBindVertexArray(0);
    

    glUseProgram(patrick_program);
    identity(patrick_transMat);
    identity(patrick_viewMat);
    lookAt(patrick_viewMat, eye, look, up);
    perspective(patrick_projMat,90, aspect, 0.1f, 1000.0f);
    rotatey(patrick_transMat, patrick_transMat, rotAngle);
    translate(patrick_transMat,patrick_transMat,testvektor); 
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
    glUniform3f(patrick_maLocation, 1.0f, 0.5f, 0.31f);
    glUniform3f(patrick_mdLocation, 1.0f, 0.5f, 0.31f);
    glUniform3f(patrick_msLocation, 0.5f, 0.5f, 0.5f);
    glUniform1f(patrick_mshineLocation, 64.0f);
    GLint patrick_laLocation = glGetUniformLocation(patrick_program, "light.ambient");
    GLint patrick_ldLocation = glGetUniformLocation(patrick_program, "light.diffuse");
    GLint patrick_lsLocation = glGetUniformLocation(patrick_program, "light.specular");
    
    glUniform3f(patrick_laLocation, 0.5f,0.0f,0.0f);
    glUniform3f(patrick_ldLocation, 0.2f,0.0f,0.0f);
    glUniform3f(patrick_lsLocation, 1.0f,1.0f,1.0f);
    glBindVertexArray(patrickVAO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    //glCullFace(GL_CULL_FACE);
    glDrawArrays(GL_TRIANGLES, 0,patrick_vertNum);
     
    
    if(rotAngle < 360) {
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
        testvektor[1] = testvektor[1] +0.5f;
    else if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
        testvektor[1] = testvektor[1] -0.5f;  
    else if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT)
        testvektor[2] = testvektor[2] +0.5f; 
        
    else if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT)
        testvektor[2] = testvektor[2] -0.5f; 
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