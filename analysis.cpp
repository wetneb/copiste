#include "analysis.h"

#include <cmath>

uint16_t* naiveDFT(uint16_t* source, int power)
{
    int size = pow(2, power);
    uint16_t* dest = new uint16_t[size];

    for(int i = 0; i != size; ++i)
    {
        dest[i] = 0;
        for(int j = 0; j != size; ++j)
        {
            dest[i] += 10.0*source[j]*sin(-2*M_PI*j*i/size);
        }
    }

    return dest;
}

