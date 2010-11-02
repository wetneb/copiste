#include "analysis.h"

#include <cmath>

int abs(int x) { return (x >= 0 ? x : -x); }

uint16_t* naiveDFT(uint16_t* source, int power)
{
    int size = pow(2, power);
    double bufRe, bufIm;
    uint16_t* dest = new uint16_t[size];

   // for(int i = 0; i != size; ++i)
   //     dest[i] = pow(2, 8)*(1 + sin(2.0*M_PI*i/size));

    for(int i = 0; i != size; ++i)
    {
        bufRe = 0;
        float pulse = -2.0*M_PI*i/size;
        for(int j = 0; j != size; ++j)
            bufRe += (source[j])*cos(pulse*j);
        bufIm = 0;
        for(int j = 0; j != size; ++j)
            bufRe += (source[j])*sin(pulse*j);
        dest[i] = sqrt(bufRe*bufRe + bufIm*bufIm);
    }

    return dest;
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

