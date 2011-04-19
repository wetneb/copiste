#include "features/spectrum.h"

#include <cmath>

//#include <iostream> // Delete me !

bool SpectrumExtr::extract(uint16_t* data, int size)
{
    // Size checks : if the size changed, we have to reallocate the buffer
    size = regularSize(size);
    if(size != mSize)
    {
        mSize = size;
        reallocate();
    }

    int* re = new int[size];
    int* im = new int[size];

    // Initially, the spectrum is set to the signal
    for(int i = 0; i < mSize; i++)
    {
        int j = mButterfly[i];
        float window = (j <= size/2 ? (2.0*j/size) : (1 - 2.0*(j - size/2)/size));
        re[i] = window*data[j];
        im[i] = 0;
    }

    int n = log2(size); // The number of recursions

    // Declared outside the loop (to speed up the code)
    int stepSize = 2; // The current size of the recursion
    float pulse = 0; // The current pulsation (2*Pi/p)
    int reP = 0, reQ = 0, imP = 0, imQ = 0; // The real and imaginary parts of the sub results of the D&C steps
    float cosOmega = 0, sinOmega = 0; // The cosine and sine of pulse*i
    float qOmegaRe = 0, qOmegaIm = 0; // The common term in the computations
    //float reduceFactor = 0.1;

    // Loop through the power of 2, size of the "recursion"
    for(int p = 1; p <= n; ++p)
    {
        stepSize = (1 << p); // 2 ^ p
        pulse = 2*M_PI/stepSize;

        // Loop through the many "recursion" of this size
        for(int start = 0; start < size; start += stepSize)
        {
            // Compute the step
            for(int i = start; i < start + stepSize/2; ++i)
            {
                reP = re[i] / 2;
                reQ = re[i+stepSize/2] / 2;
                imP = im[i] / 2;
                imQ = im[i+stepSize/2] / 2;
                cosOmega = cos(pulse*(i-start));
                sinOmega = sin(pulse*(i-start));
                qOmegaRe = reQ*cosOmega - sinOmega*imQ;
                qOmegaIm = imQ*cosOmega + reQ*sinOmega;

                re[i] = (reP + qOmegaRe);
                im[i] = (imP + qOmegaIm);
                re[i+stepSize/2] = (reP - qOmegaRe);
                im[i+stepSize/2] = (imP - qOmegaIm);
            }
        }
    }

    // Compute the module of the complex values, with butterfly
    for(int i = 0; i < size; i++)
        mResults[i] = re[i]*re[i] + im[i]*im[i];


    delete re;
    delete im;
    // */
    /*
    double bufRe, bufIm;
    long long currentVal;

    uint16_t ref = -1;
    ref /= 2;

    // Actual computation using the formula
    for(int i = 0; i != size/2; ++i)
    {
        bufRe = 0;
        float pulse = -2.0*M_PI*i/(size);
        for(int j = 0; j != size; ++j)
        {
            float window = (j <= size/2 ? (2.0*j/size) : (1 - 2.0*(j - size/2)/size));
            currentVal = (data[j]/16)*window;// - ref;
            bufRe += currentVal*cos(pulse*j);
        }

        bufIm = 0;
        for(int j = 0; j != size; ++j)
        {
            float window = (j <= size/2 ? (2.0*j/size) : (1 - 2.0*(j - size/2)/size));
            currentVal = data[j]*window/16;// - ref;
            bufRe += currentVal*sin(pulse*j);
        }

        mResults[i] = (bufRe/100.0)*(bufRe/100.0) + (bufIm/100.0)*(bufIm/100.0); //1000.0*log(bufRe*bufRe + bufIm*bufIm);
    }
    // */
    return true;
}

SpectrumExtr::SpectrumExtr(int size) : mResults(0), mButterfly(0), mSize(size)
{
    reallocate();
}

void SpectrumExtr::reallocate()
{
    // n = log2(size)
    // This code ensures we are using an exact power of 2 as size
    int n = 0;
    while(mSize > 1)
    {
        n++;
        mSize >>= 1;
    }
    mSize = (1 << n);

    if(mSize != 0)
        mResults = new uint16_t[mSize];

    // Butterfly computation
    if(mButterfly != 0)
        delete mButterfly;

    mButterfly = new int[mSize];
    int pos, res;

    for(int i = 0; i < mSize; ++i)
    {
        pos = 0;
        res = i;
        for(int p = 1; p <= n; ++p)
        {
            pos += ((res%2) << (n-p)); // pos += pow(2,n-p) * (res mod 2)
            res = (res >> 1); // res = res / 2
        }
        mButterfly[pos] = i;
    }
}

void SpectrumExtr::normalize(int bound)
{
    // Find the maximum
    uint16_t max = 0;
    for(int i = 0; i < mSize; i++)
        if(mResults[i] > max)
            max = mResults[i];

    // Divide
    float ratio = ((float) bound) / max;
    for(int i = 0; i < mSize; i++)
        mResults[i] *= ratio;

}

float SpectrumExtr::value(int index)
{
    if(index < mSize)
        return mResults[index];
    return 0;
}

uint16_t* SpectrumExtr::spectrum()
{
    return mResults;
}

int SpectrumExtr::size()
{
    return mSize;
}

int SpectrumExtr::regularSize(int size)
{
    return (1 << log2(size));
}

int SpectrumExtr::log2(int x)
{
    int n = 0;
    while(x > 1)
    {
        n++;
        x >>= 1;
    }
    return n;
}

SpectrumExtr::~SpectrumExtr()
{
    if(mResults)
        delete mResults;
    if(mButterfly)
        delete mButterfly;
}


