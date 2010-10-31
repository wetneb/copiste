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

