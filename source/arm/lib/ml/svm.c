 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define N_FEATURES 85
#define N_CLASSES 5
#define N_VECTORS 889
#define N_ROWS 5
#define N_COEFFICIENTS 4
#define N_INTERCEPTS 10
#define KERNEL_TYPE 'r'
#define KERNEL_GAMMA scale
#define KERNEL_COEF 0.0
#define KERNEL_DEGREE 3

//vectors and coefs should be loaded from external-storage(flash/sd card).
double vectors[889][85];// = {{2.52104694027002e+16, 2.740665200927678e+16, 2.52104694027002e+16, 1.9615119185509284e+16, 1.3083109378444922e+16, 8837147436750856.0, ... 8680688263533926.0, 1.1908271652423966e+16, 1.6082514306048242e+16, 1.861714249998807e+16, 1.8325986099346816e+16,  -6160906508125026.0, -6265771257091101.0}};
double coefficients[4][889];// = {{1.0, 1.0, 1.0, 0.0, 1.0 ... -1.0, -1.0}};
double intercepts[10];// = {0.21222740539356494, 0.3919845255304607, 0.5414474557663419, 0.08933456324421757, 0.6803465402359066, 0.3414555719946708, 0.026307382778791553, 0.418430390680496, -0.021166888229613788, -0.5517246150623868};
int weights[5];// = {155, 181, 167, 186, 200};

void load_data_coefs(){
	// load vectors, coefs,intercepts and weights from external-storage
	// ...
}

int predict (double features[]) {
    int i, j, k, d, l;

    double kernels[N_VECTORS];
    double kernel;
    switch (KERNEL_TYPE) {
        case 'l':
            // <x,x'>
            for (i = 0; i < N_VECTORS; i++) {
                kernel = 0.;
                for (j = 0; j < N_FEATURES; j++) {
                    kernel += vectors[i][j] * features[j];
                }
                kernels[i] = kernel;
            }
            break;
        case 'p':
            // (y<x,x'>+r)^d
            for (i = 0; i < N_VECTORS; i++) {
                kernel = 0.;
                for (j = 0; j < N_FEATURES; j++) {
                    kernel += vectors[i][j] * features[j];
                }
                kernels[i] = pow((KERNEL_GAMMA * kernel) + KERNEL_COEF, KERNEL_DEGREE);
            }
            break;
        case 'r':
            // exp(-y|x-x'|^2)
            for (i = 0; i < N_VECTORS; i++) {
                kernel = 0.;
                for (j = 0; j < N_FEATURES; j++) {
                    kernel += pow(vectors[i][j] - features[j], 2);
                }
                kernels[i] = exp(-KERNEL_GAMMA * kernel);
            }
            break;
        case 's':
            // tanh(y<x,x'>+r)
            for (i = 0; i < N_VECTORS; i++) {
                kernel = 0.;
                for (j = 0; j < N_FEATURES; j++) {
                    kernel += vectors[i][j] * features[j];
                }
                kernels[i] = tanh((KERNEL_GAMMA * kernel) + KERNEL_COEF);
            }
            break;
    }

    int starts[N_ROWS];
    int start;
    for (i = 0; i < N_ROWS; i++) {
        if (i != 0) {
            start = 0;
            for (j = 0; j < i; j++) {
                start += weights[j];
            }
            starts[i] = start;
        } else {
            starts[0] = 0;
        }
    }

    int ends[N_ROWS];
    for (i = 0; i < N_ROWS; i++) {
        ends[i] = weights[i] + starts[i];
    }

    if (N_CLASSES == 2) {

        for (i = 0; i < N_VECTORS; i++) {
            kernels[i] = -kernels[i];
        }

        double decision = 0.;
        for (k = starts[1]; k < ends[1]; k++) {
            decision += kernels[k] * coefficients[0][k];
        }
        for (k = starts[0]; k < ends[0]; k++) {
            decision += kernels[k] * coefficients[0][k];
        }
        decision += intercepts[0];

        if (decision > 0) {
            return 0;
        }
        return 1;

    }

    double decisions[N_INTERCEPTS];
    double tmp;
    for (i = 0, d = 0, l = N_ROWS; i < l; i++) {
        for (j = i + 1; j < l; j++) {
            tmp = 0.;
            for (k = starts[j]; k < ends[j]; k++) {
                tmp += kernels[k] * coefficients[i][k];
            }
            for (k = starts[i]; k < ends[i]; k++) {
                tmp += kernels[k] * coefficients[j - 1][k];
            }
            decisions[d] = tmp + intercepts[d];
            d = d + 1;
        }
    }

    int votes[N_INTERCEPTS];
    for (i = 0, d = 0, l = N_ROWS; i < l; i++) {
        for (j = i + 1; j < l; j++) {
            votes[d] = decisions[d] > 0 ? i : j;
            d = d + 1;
        }
    }

    int amounts[N_CLASSES];
    for (i = 0, l = N_CLASSES; i < l; i++) {
        amounts[i] = 0;
    }
    for (i = 0; i < N_INTERCEPTS; i++) {
        amounts[votes[i]] += 1;
    }

    int classVal = -1;
    int classIdx = -1;
    for (i = 0; i < N_CLASSES; i++) {
        if (amounts[i] > classVal) {
            classVal = amounts[i];
            classIdx= i;
        }
    }
    return classIdx;

}

int main(int argc, const char * argv[]) {

    /* Features: */
    double features[argc-1];
    int i;
    for (i = 1; i < argc; i++) {
        features[i-1] = atof(argv[i]);
    }

    /* Prediction: */
    printf("%d", predict(features));
    return 0;

}
