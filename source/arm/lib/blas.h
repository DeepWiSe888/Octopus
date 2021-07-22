
#ifndef INC_BLAS_H_
#define INC_BLAS_H_


#ifdef  __cplusplus
extern "C"
{
#endif

#include "task_define.h"


// ------- data defines ------- //
typedef struct _complex{
    float i;
    float q;
}complex,Complex;

//matrix complex
#define MAX_DIM_CNT	(5)
typedef struct _mat_c{
    char	mat_describe[32];//'complex'
    int 	dim_cnt;
    int 	dims[MAX_DIM_CNT];
    complex* data;    // use like this matc.data[dim3inx(i,j,k)]
}matc;


//matrix real float
typedef struct _mat_r{
    char	mat_info[32]; //'real'
    int 	dim_cnt;
    int 	dims[MAX_DIM_CNT];
    float* 	data;
}matr;


int dim2inx(int* dims, int m, int n);
int dim3inx(int* dims, int m, int n, int k);
int dim4inx(int* dims, int m, int n, int k, int p);

// mat 1/2/3/4 dims value
#define M1V(m, i)           (m->data[i])
#define M2V(m, i, j)        (m->data[dim2inx(m->dims, i,j)])
#define M3V(m, i, j, k)     (m->data[dim3inx(m->dims, i,j,k)])
#define M4V(m, i, j, k, p)  (m->data[dim4inx(m->dims, i,j,k,p)])


//m*n like : frames*range_bins
matc* createMat2C(int M, int N);
//m*n*p
matc* createMat3C(int M, int N, int K);

// create a mat as same size
matc* createSameMat(void* inMat);



int getMatSize(void* pMat);

//test
void printMat(matc* mc);














// ------- filter -------- //
#define FIR_N	(127)
// (frames_per_sec, n SHOULD BE 127, freq_pass,freq_stop, sig_length, sig_in, sig_out);
int fir(float fps,int numtaps, float fp, float fs,int len,Complex *x,Complex *y );




#ifdef  __cplusplus
}
#endif

#endif
