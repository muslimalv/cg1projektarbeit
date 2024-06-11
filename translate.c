#include <stdio.h>
#include <math.h>

void translate(float* out, float* in, float* v) {
    float T[] = {
        1.0f,0.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,0.0f,
        0.0f,0.0f,1.0f,0.0f,
        v[0],v[1],v[2],1.0f
    };
    float res[16];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
             
            float val1 =    T[i*4] * in[j*4];
            float val2 = T[i*4+1] * in[j*4+1];
            float val3 = T[i*4+2] * in[j*4+2];
            float val4 = T[i*4+3] * in[j*4+3];
            float result = val1 +  val2 + val3 + val4;
            res[i*4+j] = result;
            printf("%f\n", result);
             
        }
        
        
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            out[i*4+j] = res[i*4+j];
        }
        
    }
    

}
void normalize(float* out, float* in) {
    float x = 1/(sqrt((in[0]*in[0]) + (in[1]*in[1]) + (in[2]*in[2])));

    for (int i = 0; i < 3; i++)
    {
        out[i] = x * in[i];
    }

}
int main(void) {
    float out[] = {
        1.0f,0.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,0.0f,
        0.0f,0.0f,1.0f,0.0f,
        0.0f,0.0f,0.0f,1.0f
    };
    float vektor[] = {
        3.0f,
        4.0f,
        0.0f
    };
    float vOut[3];
    normalize(vOut,vektor);
    for (int i = 0; i < 3; i++)
    {
        printf("%f\n", vOut[i]);
    }
    
        
    
    return 0;
}