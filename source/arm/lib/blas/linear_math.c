#include "linear_math.h"
#include <math.h>


// -Standard deviation - //
datatype stdc(complex *x, int len)
{
    if(len<=1)
        return 0;

    Complex sum={0,0};
    int i;
    for(i=0;i<len;i++)
    {
        sum.i += x[i].i;
        sum.q += x[i].q;
    }
    Complex avg;
    avg.i = sum.i/len;
    avg.q = sum.q/len;

    datatype v = 0;
    Complex diff;
    for(i=0;i<len;i++)
    {
        diff.i = x[i].i - avg.i;
        diff.q = x[i].q - avg.q;
        v += (diff.i*diff.i + diff.q*diff.q);
    }

    v = v/len; // var
    v = sqrt(v);

    return v;
}

void stdc2d(matc* m, int dim, float* out)
{
    int i,j;
    if(dim==1)  // cause dim 1 is stored at continuous memory .
    {
        for(i=0;i<m->dims[0];i++)
        {
            out[i] = stdc(&M2V(m, i, 0), m->dims[1]);
        }
        return;

    }

    for(i=0;i<m->dims[1];i++)
    {
            int len = m->dims[0];
            Complex sum={0,0};
            int j;
            for(j=0;j<len;j++)
            {
                sum.i += M2V(m, j ,i).i;
                sum.q += M2V(m, j ,i).q;
            }
            Complex avg;
            avg.i = sum.i/len;
            avg.q = sum.q/len;

            datatype v = 0;
            Complex diff;
            for(j=0;j<len;j++)
            {
                diff.i = M2V(m, j ,i).i - avg.i;
                diff.q = M2V(m, j ,i).q - avg.q;
                v += (diff.i*diff.i + diff.q*diff.q);
            }

            v = v/len; // var
            v = sqrt(v);
            out[i] = v;

    }
}


int findMax(datatype* x, int len)
{
    int i;
    int maxInx = 0;
    datatype maxVal = -9e9;
    for(i=0;i<len;i++)
    {
        if(x[i]>maxVal)
        {
            maxInx = i;
            maxVal = x[i];
        }
    }
    return maxInx;
}