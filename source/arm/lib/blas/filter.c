#include "filter.h"
#include <math.h>
#include <stdlib.h>

#ifdef _LINUX
#include "fftw3.h"
#endif

#ifndef pi
#define pi  (3.0)
#endif

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
int fir_bandpass(float fps,int numtaps, float fp, float fs,int len,Complex *x,Complex *y )
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
    return 0;
#endif

    datatype *xreal = (datatype*)malloc(sizeof(datatype)*len);
    datatype *ximag = (datatype*)malloc(sizeof(datatype)*len);
    for(int i = 0;i < len;i++){
        xreal[i] = x[i].i;
        ximag[i] = x[i].q;
    }
    datatype *xr = (datatype*)malloc(sizeof(datatype)*len);
    fftconv(len,xreal,numtaps,k,xr);
    datatype *xi = (datatype*)malloc(sizeof(datatype)*len);
    fftconv(len,ximag,numtaps,k,xi);
    for(int i = 0;i < len;i++){
        y[i].i = xr[i];
        y[i].q = xi[i];
    }
    free(xreal);
    free(ximag);
    free(xr);
    free(xi);

    return 0;

}




int fftc2c(Complex* in, int len, Complex* out, int N)
{
    if(sizeof(datatype) == sizeof(float)){
        fftw_complex* cply = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
        fftw_complex* spec = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
        int i;
        for(i=0;i<N;i++){
            if(i<len)
            {
                cply[i][0] = in[i].i;
                cply[i][1] = in[i].q;
            }
            else
            {
                cply[i][0] = 0;
                cply[i][1] = 0;
            }
        }
        fftw_plan p = fftw_plan_dft_1d(N, cply, spec, FFTW_FORWARD, FFTW_ESTIMATE);
        fftw_execute(p);
        fftw_destroy_plan(p);
        for(i=0; i<N; i++){
            out[i].i = spec[i][0];
            out[i].q = spec[i][1];
        }
        fftw_free(cply);
        fftw_free(spec);
    } else if(sizeof(datatype) == sizeof(double)){
        fftw_complex* cply = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
        fftw_complex* spec = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
        int i;
        for(i=0;i<N;i++){
            cply[i][0] = in[i].i;
            cply[i][1] = in[i].q;
        }
        fftw_plan p = fftw_plan_dft_1d(N, cply, spec, FFTW_FORWARD, FFTW_ESTIMATE);
        fftw_execute(p);
        fftw_destroy_plan(p);
        for(i=0; i<N; i++){
            out[i].i = spec[i][0];
            out[i].q = spec[i][1];
        }
        fftw_free(cply);
        fftw_free(spec);
    }
    return 0;
}
//

///fft3w real to complex
int fftr2c(int n,datatype *in, int len, Complex *result){
    double *indata = (double*)malloc(sizeof(double)*n);
    for(int i = 0;i < n;i++){
        if(i < len){
            indata[i] = in[i];
        }else{
            indata[i] = 0;
        }
    }

#if usedouble //if(sizeof(datatype) == sizeof(double)){
    fftw_complex* out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);
    fftw_plan p = fftw_plan_dft_r2c_1d(n,indata,out,FFTW_ESTIMATE);
    fftw_execute (p);
    fftw_destroy_plan(p);
    for(int i = 0;i < n;i++){
        result[i].real = out[i][0];
        result[i].imag = out[i][1];
    }
    fftw_free(out);
#else //} else if(sizeof(datatype) == sizeof(float)){
    fftw_complex* out = (fftw_complex*) fftw_malloc(sizeof(fftwf_complex) * n);
    fftw_plan p = fftw_plan_dft_r2c_1d(n,indata,out,FFTW_ESTIMATE);
    fftw_execute (p);
    fftw_destroy_plan(p);
    for(int i = 0;i < n;i++){
        result[i].i = out[i][0];
        result[i].q = out[i][1];
    }
    fftw_free(out);
#endif    //}
    free(indata);
    return 0;
}

///fft3w complex to real
int ifftc2r(int N,Complex *in,int len, datatype *out){

#if usedouble //if(sizeof(datatype) == sizeof(double)){
    fftw_complex* cply = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
    for(int i=0;i<N;i++){
        if(i < len){
            cply[i][0] = in[i].real;
            cply[i][1] = in[i].imag;
        } else{
            cply[i][0] = 0;
            cply[i][1] = 0;
        }
    }
    fftw_plan p =fftw_plan_dft_c2r_1d(N, cply, out,FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);
    fftw_free(cply);
#else //} else if(sizeof(datatype) == sizeof(float)){
    fftw_complex* cply = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
    for(int i=0;i<N;i++){
        if(i < len){
            cply[i][0] = in[i].i;
            cply[i][1] = in[i].q;
        } else{
            cply[i][0] = 0;
            cply[i][1] = 0;
        }
    }
    double *tmpout = malloc(sizeof(double)*N);
    fftw_plan p =fftw_plan_dft_c2r_1d(N, cply, tmpout,FFTW_ESTIMATE);
    fftw_execute(p);
    for(int i=0;i<N;i++){
        out[i] = tmpout[i];
    }
    fftw_destroy_plan(p);
    fftw_free(cply);
    free(tmpout);
#endif    //}

    return 0;
}

void fftconv(int xlen,datatype *x,int klen, datatype *k, datatype *y){
    int s1 = xlen;
    int s2 = klen;
    int shape = s1  + s2 - 1;

    //fast fft len
    int nfft = shape;

    Complex *xfft = (Complex*)malloc(sizeof(Complex) * nfft);
    fftr2c(nfft,x,s1,xfft);

    Complex *kfft = (Complex*)malloc(sizeof(Complex) * nfft);
    fftr2c(nfft,k,s2,kfft);

    int fshap =  nfft / 2 + 1;
    Complex *mul =(Complex*)malloc(sizeof(Complex)* fshap);
    for(int i = 0;i < fshap;i++){
        Complex vtmp = xfft[i];
        Complex ktmp = kfft[i];
        Complex multmp;
        multmp.i = 0;
        multmp.q = 0;
        complex_mul(&vtmp,&ktmp,&multmp);
        mul[i].i = multmp.i;
        mul[i].q = multmp.q;
    }
    free(xfft);
    free(kfft);
    datatype *res =(datatype*)malloc(sizeof(datatype) * nfft);
    ifftc2r(nfft,mul,fshap,res);
    free(mul);

    for(int i = 0;i < nfft;i++){
        res[i] = res[i] / nfft;
    }
    int startind = (shape - s1) / 2;
    int endind = startind + s1;
    for(int i = startind;i < endind;i++){
        y[i - startind] = res[i];
    }
    free(res);
}