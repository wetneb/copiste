#include "filters/highlowratio.h"

//! Extracts the feature by counting the high/low values
void HighLowRatioFilter::transform(vector<float> data)
{
    // This code is quite efficient but we choosed not to compare all the values of the memory
    // to the new average at each iteration. It might just slow down the variations of the output.
    //! \todo Control this with a special bool parameter, or choose.

    if(mMemory.size() != data.size())
        reallocate(data.size());

    for(unsigned int i = 0; i < data.size(); i++)
    {
        mResults[i] -= (mMemory[mCurrentIndex][i] > (mBound * mAverage[i]));
        mAverage[i] -= mMemory[mCurrentIndex][i];

        mMemory[mCurrentIndex][i] = data[i];

        mAverage[i] += data[i];
        mResults[i] += (data[i] > (mBound * mAverage[i]));
    }

    mCurrentIndex = (mCurrentIndex + 1) % mWindowSize;
}

//! Set a float parameter (available : bound)
void HighLowRatioFilter::setFloat(string key, float value)
{
    if(key == "bound")
        mBound = value;
}

//! Set a int parameter (available : high, windowSize)
void HighLowRatioFilter::setInt(string key, int value)
{
    if(key == "high")
        mHigh = (value != 0);
    else if(key == "windowSize")
        mWindowSize = value;
}

//! Get a float parameter (available : bound)
float HighLowRatioFilter::getFloat(string key)
{
    float result = 0;
    if(key == "bound")
        result = mBound;
    return result;
}

//! Get a int parameter (available : high, windowSize)
int HighLowRatioFilter::getInt(string key)
{
    int result = 0;
    if(key == "high")
        result = mHigh;
    else if(key == "windowSize")
        result = mWindowSize;
    return result;
}

void HighLowRatioFilter::reallocate(unsigned int newSize)
{
    // If newSize < current size, delete
    for(unsigned int i = newSize; i < mMemory.size(); i++)
        delete mMemory[i];

    if(newSize < mMemory.size())
    {
        mMemory.resize(newSize);
        mAverage.resize(newSize);
        mResults.resize(newSize);
    }

    // If newSize > currentSize, new
    for(unsigned int i = mMemory.size(); i < newSize; i++)
    {
        mMemory.push_back(new float[mWindowSize]);
        mAverage.push_back(0);
        mResults.push_back(0);
    }
}

HighLowRatioFilter::~HighLowRatioFilter()
{
    reallocate(0);
}

