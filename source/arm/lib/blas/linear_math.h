#ifndef _BLAS_MATH_H
#define _BLAS_MATH_H


#ifdef  __cplusplus
extern "C"
{
#endif

#include "blas.h"

// -Standard deviation - //
datatype stdc(complex *x, int len);
void stdc2d(matc* m, int dim, datatype* out);


int findMax(datatype* x, int len);


#ifdef  __cplusplus
}
#endif

#endif