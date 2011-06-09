#include "filters/flux.h"

#include <cmath>

//! Compute the transformation : compute the "flux"
void Flux::transform(vector<float> data)
{
    if(data.size() == mLastVect.size())
    {
        if(mResult.size() != data.size())
            mResult.resize(data.size());
        for(unsigned int i = 0; i < data.size(); i++)
            mResult[i] = abs(mLastVect[i] - data[i]);
    }

    mLastVect = data;
}

//! Get the number of available values (the same as in the original feature)
int Flux::size()
{
    int result = 0;
    if(mExtr)
        result = mExtr->size();
    return result;
}

//! Return the asked value (indices are the same as in the original feature)
float Flux::value(int index)
{
    float result = 0;
    if((unsigned int)index < mResult.size())
        result = mResult[index];
    return result;
}
