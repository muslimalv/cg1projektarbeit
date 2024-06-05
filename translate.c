#include <stdio.h>

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
int main(void) {
    float out[] = {
        1.0f,0.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,0.0f,
        0.0f,0.0f,1.0f,0.0f,
        0.0f,0.0f,0.0f,1.0f
    };
    float vektor[] = {
        2,
        2,
        3
    };

    translate(out,out,vektor);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
             printf("%f ",out[i*4+j]);    
        }
        printf("\n");
        
    }
    return 0;
}