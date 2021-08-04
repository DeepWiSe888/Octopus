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

// how to: imaging a matrix, m at Y, n at X, so m*width+n
int dim2inx(int* dims, int m, int n) {return (m)*dims[1]+n;}
// how to: imaging a cube, m at Y, n at X, k at Z, so m*area(of X*Z) and n*Z and k
int dim3inx(int* dims, int m, int n, int k) {return (m)*dims[1]*dims[2]+(n)*dims[2]+k;}
// how to: imaging cubes at different spatio then you have 4d, m at spatio dim(passed through m cubes), then n/k/p as 3d above
int dim4inx(int* dims, int m, int n, int k, int p) {return (m)*dims[1]*dims[2]*dims[3]+(n)*dims[2]*dims[3]+(k)*dims[3]+k;}


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








///sinc
float sinc(float x){
    if(x == 0 || x<1e-6)
        return 1.0;
    else
        return sin(x)/x;
}

///firwin
void firwin(int numtaps,float fp,float fs, float *h){
    float alpha = 0.5 * (numtaps - 1);
    float scale_frequency = 0;
    if(fp == 0){
        scale_frequency = 0.0;
    }else if(fs == 1){
        scale_frequency = 1.0;
    }else{
        scale_frequency = 0.5 * (fp + fs);
    }
    float s = 0;
    float temp[FIR_N];
    for(int i = 0;i < numtaps;i++){
        float m = (i - alpha);
        float hamming = 0.54 - 0.46*cos(2*pi*i/((float)numtaps - 1.0));
        float x1 = fs * m * pi;
        float x2 = fp * m * pi;

        float tt = 0.0;
        tt = tt + fs*sinc(x1);
        tt = tt - fp*sinc(x2);
        tt = tt * hamming;
        float c = cos(pi * m * scale_frequency);
        s += tt * c;
        temp[i] = tt;
    }
    for(int i = 0;i < numtaps;i++){
        h[i] = temp[i] / s;
    }
}
// (frames_per_sec, n, freq_pass,freq_stop, sig_length, sig_in, sig_out);
int fir(float fps,int numtaps, float fp, float fs,int len,Complex *x,Complex *y )
{
	// generate step window, hamming
    float npyfp = fp /(fps / 2.0);
    float npyfs = fs /(fps / 2.0);
	float k[FIR_N];
	firwin(FIR_N, npyfp, npyfs, k);

#ifdef _ARM_SIMD
	// init fir
	arm_fir_instance_f32 s;
	int blockSize = 128;
	float* state = (float*)taskMemAlloc(sizeof(float)*(2 * blockSize + FIR_N - 1));
	arm_fir_init_f32(&s, FIR_N, k, state, blockSize);
    int numBlocks = len/blockSize;
    int i;
    for(i=0;i<numBlocks;i++)
    {
        arm_fir_f32(&s, x + (i * blockSize), y + (i * blockSize), blockSize);
    }
	taskMemFree(state);
#endif



	return 0;

}



