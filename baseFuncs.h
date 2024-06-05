#ifndef BASEFUNCS_H
#define BASEFUNCS_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>


void identity(GLfloat* out);
void translate(GLfloat* out, GLfloat* in, GLfloat* v);
void scale(GLfloat* out, GLfloat* in, GLfloat* v);
void rotatex(GLfloat* out, GLfloat* in, GLfloat angle);
void rotatey(GLfloat* out, GLfloat* in, GLfloat angle);
void rotatez(GLfloat* out, GLfloat* in, GLfloat angle);
void normalize(GLfloat* out, GLfloat* in);
void cross(GLfloat* out, GLfloat* a, GLfloat* b);
void lookAt(GLfloat* out, GLfloat* eye, GLfloat* center, GLfloat* up);
void perspective(GLfloat* out, GLfloat fovy, GLfloat aspect, GLfloat near, GLfloat far);




#endif