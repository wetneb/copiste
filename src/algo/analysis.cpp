#include "algo/analysis.h"

#include <cmath>

int abs(int x) { return (x >= 0 ? x : -x); }

uint16_t* naiveDFT(uint16_t* source, int power)
{
    int size = pow(2, power);
    long long bufRe, bufIm, currentVal;
    uint16_t* dest = new uint16_t[size/2];

   // for(int i = 0; i != size; ++i)
   //     dest[i] = pow(2, 8)*(1 + sin(2.0*M_PI*i/size));

    uint16_t ref = -1;
    ref /= 2;

    for(int i = 0; i != size/2; ++i)
    {
        bufRe = 0;
        float pulse = -2.0*M_PI*i/(size);
        for(int j = 0; j != size; ++j)
        {
            float window = (j <= size/2 ? (2.0*j/size) : (1 - 2.0*(j - size/2)/size));
            currentVal = (source[j]/16)*window;// - ref;
            bufRe += currentVal*cos(pulse*j);
        }

        bufIm = 0;
        for(int j = 0; j != size; ++j)
        {
            float window = (j <= size/2 ? (2.0*j/size) : (1 - 2.0*(j - size/2)/size));
            currentVal = source[j]*window/16;// - ref;
            bufRe += currentVal*sin(pulse*j);
        }

        dest[i] = (bufRe/100.0)*(bufRe/100.0) + (bufIm/100.0)*(bufIm/100.0); //1000.0*log(bufRe*bufRe + bufIm*bufIm);
    }

    return dest;
}

int* butterfly(int n)
{
    int size = (1 << n);
    int *result = new int[size];
    for(int i = 0; i < size; ++i)
    {
        int pos = 0, res = i;
        for(int p = 1; p <= n; ++p)
        {
            pos += ((res%2) << (n-p)); // pos += pow(2,n-p) * (res mod 2)
            res = (res >> 1); // res = res / 2
        }
        result[pos] = i;
    }

    return result;
}

float ZCR(uint16_t* source, unsigned int size)
{
    uint16_t limit = pow(2,15);
    unsigned int zeros = 0;
    bool positive = source[0] > limit;
    for(unsigned int i = 1; i != size; ++i)
    {
        if((source[i] > limit) == !positive)
        {
            ++zeros;
            positive = !positive;
        }
    }
    return ((float)zeros)/size;
}

