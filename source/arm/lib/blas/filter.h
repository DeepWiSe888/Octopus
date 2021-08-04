#ifndef _BLAS_FILTER_H
#define _BLAS_FILTER_H


#ifdef  __cplusplus
extern "C"
{
#endif

#include "blas.h"



// ------- filter -------- //
#define FIR_N	(127)
// (frames_per_sec, n SHOULD BE 127, freq_pass,freq_stop, sig_length, sig_in, sig_out);
int fir_bandpass(float fps,int numtaps, float fp, float fs,int len,Complex *x,Complex *y );



// ------- FFT -------- //

int fftc2c(Complex* x, int len, Complex* y, int N);

///fft3w real to complex
int fftr2c(int n,datatype *in, int len, Complex *result);

///fft3w complex to real
int ifftc2r(int N,Complex *in,int len, datatype *out);

///frequency domain convolute
void fftconv(int xlen,datatype *x,int klen, datatype *k, datatype *y);



#ifdef  __cplusplus
}
#endif

#endif