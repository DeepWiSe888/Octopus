/*
 * Project Octopus
 *
 */



#include "blas.h"
#include <memory.h>
#include "task_define.h"
#include "captain.h"
#include <math.h>
#include <stdint.h>

#ifdef _ARM_SIMD
//#include <arm_math.h>
#endif

#ifndef pi
#define pi	(3.14)
#endif


// Operator+
void complex_add(complex* a, complex* b, complex* out)
{
    out->i=a->i+b->i;
    out->q=a->q+b->q;
}
// Operator-
void complex_sub(complex* a, complex* b, complex* out)
{
    out->i=a->i-b->i;
    out->q=a->q-b->q;
}
// Operator*
void complex_mul(complex* a, complex* b, complex* out)
{
    out->i=a->i*b->i - a->q*b->q;
    out->q=a->i*b->q + a->q*b->i;
}

void complex_mul_expjx(complex*a, datatype x, complex* out)
{
    //e^jx = cos(x)+jsin(x);
    complex c2;
    c2.i= cos(x);
    c2.q = sin(x);

    complex_mul(a, &c2, out);

}

datatype complex_abs(complex* a)
{
    datatype amp = sqrt(a->i*a->i + a->q*a->q);
    return amp;
}
datatype complex_angle(complex* a)
{
    datatype ph = atan2(a->q, a->i);
    return ph;
}



// how to: imaging a matrix, m at Y, n at X, so m*width+n
int dim2inx(int* dims, int m, int n) {return (m)*dims[1]+n;}
// how to: imaging a cube, m at Y, n at X, k at Z, so m*area(of X*Z) and n*Z and k
int dim3inx(int* dims, int m, int n, int k) {return (m)*dims[1]*dims[2]+(n)*dims[2]+k;}
// how to: imaging cubes at different spatio then you have 4d, m at spatio dim(passed through m cubes), then n/k/p as 3d above
int dim4inx(int* dims, int m, int n, int k, int p) {return (m)*dims[1]*dims[2]*dims[3]+(n)*dims[2]*dims[3]+(k)*dims[3]+p;}


//vector infact
matc* createMat1C(int M)
{
    complex * buf = (complex*)taskMemAlloc(M*sizeof(complex));
    bzero(buf, M*sizeof(complex));

    matc *mc = (matc*)taskMemAlloc(sizeof(matc));
    bzero(mc, sizeof(matc));
    strcpy(mc->mat_describe, "complex");
    mc->dim_cnt = 1;
    mc->dims[0] = M;
    mc->data = buf;

    return mc;
}

matc* createMat2C(int M, int N)
{
    complex * buf = (complex*)taskMemAlloc(M*N*sizeof(complex));
    bzero(buf, M*N*sizeof(complex));

    matc *mc = (matc*)taskMemAlloc(sizeof(matc));
    bzero(mc, sizeof(matc));
    strcpy(mc->mat_describe, "complex");
    mc->dim_cnt = 2;
    mc->dims[0] = M;
    mc->dims[1] = N;
    mc->data = buf;

    return mc;
}


matc* createMat3C(int M, int N, int P)
{
    complex * buf = (complex*)taskMemAlloc(M*N*P*sizeof(complex));
    bzero(buf, M*N*P*sizeof(complex));

    matc *mc = (matc*)taskMemAlloc(sizeof(matc));
    bzero(mc, sizeof(matc));
    strcpy(mc->mat_describe, "complex");
    mc->dim_cnt = 3;
    mc->dims[0] = M;
    mc->dims[1] = N;
    mc->dims[2] = P;
    mc->data = buf;

    return mc;
}


// create a mat as same size
matc* createSameMat(void* _inMat)
{
    matc* inMat = (matc*)_inMat;
    matc *mc = (matc*)taskMemAlloc(sizeof(matc));
    bzero(mc, sizeof(matc));
    memcpy(mc, inMat, (size_t)(&(((matc*)0)->data))); //copy size before data

    int bufsize = 1;
    int i;
    for(i=0;i<inMat->dim_cnt;i++)
        bufsize = bufsize * inMat->dims[i];

    complex * buf = (complex*)taskMemAlloc(bufsize*sizeof(complex));
    bzero(buf, bufsize*sizeof(complex));
    mc->data = buf;
    return mc;
}

int getMatSize(void* pMat)
{
    return sizeof(matc);
}


#include <stdio.h>
void printMat(matc* mc, const char* name)
{
    if(name)
        printf("%s = [", name);
    else
        printf("mat = [");
    int i;
    complex * x = &M1V(mc, 0);
    for (i = 0; i < mc->dims[0]; i++) {
        printf("%f+%fi ", x[i].i, x[i].q);
    }
    printf("];\n");
}