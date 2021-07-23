/*
 * Project Octopus
 *
 */


#include "VMD.h"



#include <vector>
#include <ctime>
#include <exception>
#include <math.h>
#include "eigen3/Eigen/Dense"
#include "eigen3/Eigen/Core"
#include "eigen3/unsupported/Eigen/FFT"


#define eps_for_VMD 2.2204e-16
using namespace Eigen;
typedef std::vector<double> vectord;
typedef std::vector<std::complex<double> > vectorcd;
typedef std::vector<MatrixXcd> Matrix3DXd;

vectorcd circshift(vectorcd& data, int offset);

template<typename T>
void reverse(T& v, int s, int l);

vectord omega_init_method2(int K, double fs);

MatrixXcd vector_to_MatrixXcd_in_row(vectorcd& Input);
MatrixXcd vector_to_MatrixXcd_in_col(vectorcd& Input);

vectorcd ExtractColFromMatrixXcd(MatrixXcd& Input, int k, int T);
vectorcd ExtractRowFromMatrixXd(MatrixXd& Input, int k, int T);

MatrixXcd sum(Matrix3DXd& u_hat_plus, int n);




void VMD(MatrixXd& u, MatrixXcd& u_hat, MatrixXd& omega,
	vectord& signal,
	double alpha, double tau, const int K, const int DC, const int init, double tol) {
	/* ---------------------
	    <VMD_CPP: C++ implementation of Variational Mode Decomposition using Eigen.>
		Copyright (C) <2019>  <Lang He: asdsay@gmail.com>
		Output:
		-------
		u - the collection of decomposed modes (2D double Matrix in Eigen -MatrixXd)
		u_hat - spectra of the modes (2D complex<double> Matrix in Eigen -MatrixXd)
		omega - estimated mode center - frequencies (2D double Matrix in Eigen -MatrixXd)
		-------
		Input:
		-------
		signal - the time domain signal(1D vector) to be decomposed
		alpha - the balancing parameter of the data - fidelity constraint
		tau - time - step of the dual ascent(pick 0 for noise - slack)
		K - the number of modes to be recovered
		DC - true if the first mode is putand kept at DC(0 - freq)
		init - 0 = all omegas start at 0
		                   1 = all omegas start uniformly distributed
		                   2 = all omegas initialized randomly
		tol - tolerance of convergence criterion; typically around 1e-6
		
	*/

	// ----------Preparations
	// Periodand sampling frequency of input signal
	int T = signal.size();
	int saveT = T;
	double fs = 1.0 / T;

	//extend the signal by mirroring
	vectord  f(2 * T, 0);
	copy(signal.begin(), signal.end(), f.begin() + T / 2);
	for (int i = 0; i < T / 2; i++)
		f[i] = signal[T / 2 - 1 - i];
	for (int i = 3 * T / 2; i < 2 * T; i++)
		f[i] = signal[T + 3 * T / 2 - 1 - i];

	// Time Domain 0 to T (of mirrored signal)
	// Spectral Domain discretization
	T = f.size();
	vectorcd freqs(T, 0);
	vectord timevec(T, 0);
	for (int i = 0; i < T; i ++) {
		timevec[i] = double(i + 1.0) / T;
		freqs[i] = (timevec[i] - 0.5) - double(1 / T);
	}

	// Maximum number of iterations(if not converged yet, then it won't anyway)
	int N = 500;

	// For future generalizations : individual alpha for each mode
	vectord Alpha = vectord(K, alpha);

	// Construct and center f_hat
	vectorcd freqvec(T);
	FFT<double> fft; fft.fwd(freqvec,f);
	vectorcd f_hat = circshift(freqvec, T / 2);
	vectorcd f_hat_plus(f_hat.size(), 0);
	copy(f_hat.begin() + T / 2, f_hat.end(), f_hat_plus.begin() + T / 2);

	// matrix keeping track of every iterant // could be discarded for mem
	Matrix3DXd u_hat_plus(N, MatrixXcd::Zero(K, T));

	// Initialization of omega_k
	MatrixXcd omega_plus(N, K);
	vectord tmp;
	switch (init) {
	case 1:
		for (int i = 0; i < K; i++) 
			omega_plus(0, i) = double(0.5 / K) * (i);
		break;
	case 2:
		tmp = omega_init_method2(K, fs);
		for (int i = 1; i <= K; i++)
			omega_plus(0, i) = tmp[i];
		break;
	default:
		break;
	}

	//% if DC mode imposed, set its omega to 0
	if (DC)
		omega_plus(0, 0) = 0;

	// start with empty dual variables
	MatrixXcd lambda_hat(N, T);

	// other inits
	double uDiff = tol + eps_for_VMD;//% update step
	int n = 1;// loop counter
	MatrixXcd sum_uk(1, T); 
	// accumulator
	int k ;
	//vectord sum_uk(freqs.size());


	// ----------- Main loop for iterative updates
	while (uDiff > tol&& n < N) {

		//update first mode accumulator
		k = 1;
		sum_uk = u_hat_plus[n - 1].row(K - 1) + sum_uk - u_hat_plus[n-1].row(0);
		
		//update spectrum of first mode through Wiener filter of residuals
		MatrixXcd Dividend_vec= vector_to_MatrixXcd_in_col(f_hat_plus) - sum_uk - (lambda_hat.row(n - 1) / 2.0);
		MatrixXcd Divisor_vec = (1 + Alpha[k - 1] *
			((vector_to_MatrixXcd_in_col(freqs).array() - omega_plus(n - 1, k - 1))).array().square());
		u_hat_plus[n].row(k-1) = Dividend_vec.cwiseQuotient(Divisor_vec);

		//update first omega if not held at 0
		if (!DC) {
			std::complex<double> Dividend(0,0), Divisor(0,0), Addend;
			for (int i = 0; i < T - T / 2; i++) {
				Addend = abs(u_hat_plus[n](k - 1, T / 2 + i))* abs(u_hat_plus[n](k - 1, T / 2 + i));
				Divisor += Addend;
				Dividend += freqs[T / 2 + i] * Addend;
			}
			omega_plus(n, k - 1) = Dividend/ Divisor;
			
		}
		// Dual ascent

		for (k = 1; k < K ; k++) {
			//accumulator
			sum_uk = u_hat_plus[n].row(k - 1) + sum_uk - u_hat_plus[n - 1].row(k);

			//mode spectrum
			MatrixXcd Dividend_vec = vector_to_MatrixXcd_in_col(f_hat_plus) - sum_uk - (lambda_hat.row(n - 1) / 2.0);
			MatrixXcd Divisor_vec = (1 + Alpha[k] *
				((vector_to_MatrixXcd_in_col(freqs).array() - omega_plus(n - 1, k))).array().square());
			u_hat_plus[n].row(k) = Dividend_vec.cwiseQuotient(Divisor_vec);

			//center frequencies
			std::complex<double> Dividend(0,0), Divisor(0,0), Addend;
			for (int i = 0; i < T - T / 2; i++) {
				Addend = abs(u_hat_plus[n](k, T / 2 + i))* abs(u_hat_plus[n](k, T / 2 + i));
				Divisor += Addend;
				Dividend += freqs[T / 2 + i] * Addend;
			}
			omega_plus(n, k) = Dividend/ Divisor;
		}
	
		lambda_hat.row(n) = lambda_hat.row(n - 1) +	tau * 
			(sum(u_hat_plus, n) - vector_to_MatrixXcd_in_col(f_hat_plus));
		n++;
		//uDiff = eps_for_VMD;

		std::complex<double> acc(eps_for_VMD, 0);
		for (int i = 0; i < K; i++) {
			MatrixXcd tmp = u_hat_plus[n-1].row(i) - u_hat_plus[n-2].row(i);
			tmp =  (tmp * (tmp.adjoint()));
			acc = acc + tmp(0,0) / double(T);

		}
		uDiff = abs(acc);

	}

	// ------ Postprocessing and cleanup

	// discard empty space if converged early
	N = std::min(N, n);
	omega = omega_plus.topRows(N).real();

	//Signal reconstruction
	u_hat = MatrixXcd::Zero(T, K);
	for (int i = T / 2; i < T; i++)
		for (int k = 0; k < K; k++)
			u_hat(i, k) = u_hat_plus[N-1](k, i);
	
	for (int i = T / 2; i >= 0; i--)
		for (int k = 0; k < K; k++) 
			u_hat(i, k) = conj(u_hat_plus[N - 1](k, T - i - 1));

			
	u_hat.row(0) = u_hat.row(N - 1).transpose().adjoint();
	u.resize(K, saveT);
	vectord result_col;
	for (int k = 0; k < K; k++) {
		vectorcd u_hat_col = ExtractColFromMatrixXcd(u_hat, k, T);
		u_hat_col = circshift(u_hat_col, int(floor(T / 2)));
		fft.inv(result_col, u_hat_col);
		for (int t = 0; t < saveT; t++)
			u(k, t) = result_col[t + T / 4];
	}


	u_hat.fill(0);
	vectord result_timevec(saveT, 0);
	for (int i = 0; i < saveT; i += 1) {
		result_timevec[i] = double(i + 1) / saveT;
	}

	for (int k = 0; k < K; k++) {
		vectorcd u_row = ExtractRowFromMatrixXd(u, k, saveT);
		fft.inv(result_timevec, u_row);
		u_row = circshift(u_row, saveT / 2);
		for (int t = 0; t < saveT; t++)
			u(k,t) = u_row[t].real();
	}

//*/
	return;
}


vectorcd circshift(vectorcd& data, int offset) {
	int n = data.size();
	offset = offset % n;
	if (offset == 0) {
		vectorcd out_data(n);
		copy(data.begin(), data.end(), out_data.begin());
		return data;
	}
	if (offset < 0)//��ǰ�ƶ�-offset��Ԫ�� =����ƶ�(n-offset)��Ԫ��
		offset = n + offset;
	vectorcd out_data(data);
	reverse(out_data,0,n-1);
	reverse(out_data, 0, n - offset - 1);
	reverse(out_data, n - offset, n - 1);
	return out_data;
}



template<typename T>
void reverse(T& v, int s, int l) {
	int n = v.size();
	auto temp = v[0];
	for (int i = s, j = l; i < j; i++, j--) {
		temp = v[i];
		v[i] = v[j];
		v[j] = temp;
	}
	return;
}
vectord omega_init_method2(int K, double fs) {
	vectord res(K, 0);
	int N = INT_MAX/2;
	srand(int(time(NULL)));
	for (int i = 0; i < K; i++) {
		res[i] = exp(log(fs) + (log(0.5) - log(fs)) *
			(rand() % (N + 1) / (float)(N + 1))
		);
	}
	sort(res.begin(), res.end());
	return res;
}

MatrixXcd vector_to_MatrixXcd_in_row(vectorcd& Input) {
	int m = Input.size();
	MatrixXcd cov(m, 1);
	for (int i = 0; i < m; ++i)
		cov(i, 0) = Input[i];
	return cov;
}

MatrixXcd vector_to_MatrixXcd_in_col(vectorcd& Input) {
	int T = Input.size();
	MatrixXcd tmp(1, T);
	for (int t = 0; t < T; t++)
		tmp(0, t) = Input[t];
	return tmp;
}

vectorcd ExtractColFromMatrixXcd(MatrixXcd& Input, int ColIdx, int RowNum) {
	vectorcd Output(RowNum, 0);
	for (int i = 0; i < RowNum; ++i)
		Output[i] = Input(i, ColIdx);
	return Output;
}

vectorcd ExtractRowFromMatrixXd(MatrixXd& Input, int RowIdx, int ColNum) {
	vectorcd Output(ColNum, 0);
	for (int i = 0; i < ColNum; ++i)
		Output[i] = Input(RowIdx, i);
	return Output;
}

MatrixXcd sum(Matrix3DXd& u_hat_plus, int n) {
	MatrixXcd cov = u_hat_plus[n];
	return cov.colwise().sum();
}


int emptyFunction(MatrixXd& u, MatrixXcd& u_hat, MatrixXd& omega, vectord& x, double alpha, double tau, const int K, const int DC, const int init, double tol)
{
	printf("K=%d, alpha=%f\n", K, alpha);
	return 0;
}


float randData()
{
    return (1000.0+rand()%5000)/(1000*1000);
}
void testVMD()
{
	int len = 800;
    vectord x(len);
    float freqRPM = 12./60.;
    float freqBPM = 70./60.;
    int i;
    float fps = 20;
    for(i=0;i<len;i++)
    {
    	x[i] = randData() + 0.5*sin(freqRPM*i/fps) + 0.1*cos(freqBPM*i/fps);
    }

    double alpha = 2000, tau = 0, tol = 1e-7;
    int K = 3, DC = 0, init = 1;
    MatrixXd u, omega;
    MatrixXcd u_hat;
    int aaa = x.size();
    emptyFunction(u, u_hat, omega, x, alpha, tau, K, DC, init, tol);
    VMD(u, u_hat, omega, x, alpha, tau, K, DC, init, tol);

}


int VMD_vital(float* data, int len, float* outBreath, float* outHeart)
{
	MatrixXd u;
	MatrixXcd u_hat;
	MatrixXd omega;
	vectord signal;
	double alpha = 0;
	double tau = 0;
	int K = 0;
	int DC = 0;
	int init = 0;
	double tol = 0;
    VMD(u, u_hat, omega,
        signal, alpha, tau, K, DC, init, tol);
	return 0;
}
