/*
 * This file is part of Copiste.
 *
 * Copiste is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 *  Copiste is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Copiste.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "features/spectrum.h"

#include <cmath>

// In this file, the most important functions are shown at first (we don't follow the order of the header)

bool SpectrumExtr::extract(int16_t* data, int size, int channels)
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
        re[i] = mWindowCache[j] * data[channels*j];
        im[i] = 0;
    }

    int n = log2(size); // The number of recursions

    // Declared outside the loop (to speed up the code)
    int stepSize = 2; // The current size of the recursion
    double pulse = 0; // The current pulsation (2*Pi/p)
    int reP = 0, reQ = 0, imP = 0, imQ = 0; // The real and imaginary parts of the sub results of the D&C steps
    double cosOmega = 0, sinOmega = 0; // The cosine and sine of pulse*i
    double qOmegaRe = 0, qOmegaIm = 0; // The common term in the computations
    //double reduceFactor = 0.1;

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
    return true;
}

// This is the default window. It doesn't change the signal.
// # # # # # # # # #
// #               #
// #               #
// #               #
// #               #
inline double SpectrumExtr::rectangularWin(int i, int maxSize)
{
    return (i >= 0 && i < maxSize) ? 1.0 : 0;
}

// This is a basic window :
//         #
//       #   #
//     #       #
//   #           #
// #               #
inline double SpectrumExtr::triangularWin(int i, int maxSize)
{
    return (i >= 0 && i < maxSize) ? (i <= maxSize/2 ? (2.0*i/maxSize) : (1 - 2.0*(i - maxSize/2)/maxSize)) : 0;
}

// A window more powerful
//
//           #  #
//       #         #
//     #             #
// # #                 # #
//
inline double SpectrumExtr::hammingWin(int i, int maxSize)
{
    return (i >= 0 && i < maxSize) ? (0.54 - 0.46*cos(2*M_PI*i/maxSize)) : 0;
}

// Another composite of sinus waves, showing good results
//
//           #  #
//       #          #
//     #              #
//   #                  #
// #                      #
inline double SpectrumExtr::blackmanHarrisWin(int i, int maxSize)
{
    return 0.35875  - 0.48829*cos(2*M_PI*i/(maxSize - 1)) + 0.14128*cos(4*M_PI*i/(maxSize - 1)) + 0.0106411*cos(6*M_PI*i/(maxSize-1));
}

SpectrumExtr::SpectrumExtr(int size) : mResults(0), mButterfly(0), mWindowCache(0), mCurrentWindow(WINDOW_HAMMING), mSize(size), mBound((1 << 31))
{
    reallocate();
}

void SpectrumExtr::reallocate()
{
    // This code ensures we are using an exact power of 2 as size
    int n = log2(size());
    mSize = pow2(n);

    if(mSize != 0)
        mResults = new uint16_t[mSize];

    // Butterfly computation
    // We just invert the bits of each number in binary representation
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

    //! Create a new window cache
    createWindowCache();
}

void SpectrumExtr::createWindowCache()
{
    delete mWindowCache;
    mWindowCache = new double[mSize];

    for(int i = 0; i < mSize; i++)
    {
        if(mCurrentWindow == WINDOW_RECTANGULAR)
            mWindowCache[i] = rectangularWin(i, mSize);
        else if(mCurrentWindow == WINDOW_TRIANGULAR)
            mWindowCache[i] = triangularWin(i, mSize);
        else if(mCurrentWindow == WINDOW_HAMMING)
            mWindowCache[i] = hammingWin(i, mSize);
        else if(mCurrentWindow == WINDOW_BH)
            mWindowCache[i] = blackmanHarrisWin(i, mSize);
    }
}

void SpectrumExtr::normalize(int bound)
{
    // If the data is already normalized to this bound, don't change anything
    if(mBound == bound)
        return;

    // Find the maximum (the last bound may not be a maximum)
    uint16_t max = 0;
    for(int i = 0; i < mSize; i++)
        if(mResults[i] > max)
            max = mResults[i];

    // Divide
    double ratio = ((double) bound) / max;
    for(int i = 0; i < mSize; i++)
        mResults[i] *= ratio;

    mBound = bound;
}

float SpectrumExtr::value(int index)
{
    // Normalize the value (in [-1; 1])
    if(index < mSize && mBound < (1 << 31))
        return (2.0 *mResults[index] / mBound - 1.0);
    else if(index < mSize)
        return mResults[index];
    return 0;
}

//! Set a int parameter (available : "bound")
void SpectrumExtr::setInt(string key, int value)
{
    if(key == "bound")
        mBound = value;
}

//! Get a int parameter (available : "bound")
int SpectrumExtr::getInt(string key)
{
    float ret = 0;
    if(key == "bound")
        ret = mBound;
    return ret;
}

uint16_t* SpectrumExtr::spectrum()
{
    return mResults;
}

inline int SpectrumExtr::size()
{
    return mSize;
}

inline int SpectrumExtr::regularSize(int size)
{
    return (1 << log2(size));
}

inline int SpectrumExtr::log2(int x)
{
    int n = 0;
    while(x > 1)
    {
        n++;
        x >>= 1;
    }
    return n;
}

inline int SpectrumExtr::pow2(int n)
{
    return (1 << n); // bitwise computation : just shift the bit n times
}

SpectrumExtr::~SpectrumExtr()
{
    delete mResults;
    delete mButterfly;
    delete mWindowCache;
}

void SpectrumExtr::setWindow(FTWindow win)
{
    mCurrentWindow = win;
    createWindowCache();
}


