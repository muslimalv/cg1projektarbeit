#include "baseFuncs.h"
#include <math.h>
#define PI		3.14159265358979323846

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
   
    GLfloat T[] = {
        1.0f,0.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,0.0f,
        0.0f,0.0f,1.0f,0.0f,
        v[0],v[1],v[2],1.0f
    };
    float result[16];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
             //out[i*4+j] = T[i*4] * in[j*4] +  T[i*4+1] * in[j*4+1] + T[i*4+2] * in[j*4+2] + T[i*4+3] * in[j*4+3];
            result[i*4+j] = T[i*4] * in[j*4] +  T[i*4+1] * in[j*4+1] + T[i*4+2] * in[j*4+2] + T[i*4+3] * in[j*4+3];
             
        }  
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            out[i*4+j] = result[i*4+j];
             
        }  
    }
}

void scale(GLfloat* out, GLfloat* in, GLfloat* v) {
    GLfloat S[] = {
        v[0], 0.0f, 0.0f, 0.0f,
        0.0f, v[1], 0.0f, 0.0f,
        0.0f, 0.0f, v[2], 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    float result[16];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result[i*4+j] =S[i*4] * in[j*4] +  S[i*4+1] * in[j*4+1] + S[i*4+2] * in[j*4+2] + S[i*4+3] * in[j*4+3];
        }
        
        //out[i] = S[i*4] * in[0] +  S[i*4+1] * in[1] + S[i*4+2] * in[2] + S[i*4+3] * in[3];
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            out[i*4+j] = result[i*4+j];
             
        }  
    }
}
void rotatez(GLfloat* out, GLfloat* in, GLfloat angle) {
   
    GLfloat R[] = {
        cos(angle), sin(angle), 0.0f, 0.0f,
        -sin(angle), cos(angle), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    float result[16];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
             result[i*4+j] = R[i*4] * in[j*4] +  R[i*4+1] * in[j*4+1] + R[i*4+2] * in[j*4+2] + R[i*4+3] * in[j*4+3];
        }
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            out[i*4+j] = result[i*4+j];
             
        }  
    }
}

void rotatex(GLfloat* out, GLfloat* in, GLfloat angle) {
   
    GLfloat R[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cos(angle), sin(angle), 0.0f,
        0.0f, -sin(angle), cos(angle), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    float result[16];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
             result[i*4+j] = R[i*4] * in[j*4] +  R[i*4+1] * in[j*4+1] + R[i*4+2] * in[j*4+2] + R[i*4+3] * in[j*4+3];
        }
        //out[i] = R[i*4] * in[0] +  R[i*4+1] * in[1] + R[i*4+2] * in[2] + R[i*4+3] * in[3];
        
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            out[i*4+j] = result[i*4+j];
             
        }  
    }
}

void rotatey(GLfloat* out, GLfloat* in, GLfloat angle) {
 
    GLfloat R[] = {
        cos(angle), 0.0f, -sin(angle), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        sin(angle), 0.0f, cos(angle), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    float result[16];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result[i*4+j] = R[i*4] * in[j*4] +  R[i*4+1] * in[j*4+1] + R[i*4+2] * in[j*4+2] + R[i*4+3] * in[j*4+3];
        }
        //out[i] = R[i*4] * in[0] +  R[i*4+1] * in[1] + R[i*4+2] * in[2] + R[i*4+3] * in[3];
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            out[i*4+j] = result[i*4+j];
             
        }  
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