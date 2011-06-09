#include "filters/range.h"

//! Do the actual computation on the features
void Range::transform(vector<float> data)
{
    float sum = 0;
    for(int i = max(mStart, 0); i < min((int)data.size(), mEnd); i++)
        sum += data[i];
    mAverage = sum / (mEnd - mStart);
}

//! Set a int parameter
void Range::setInt(string key, int value)
{
    if(key == "start")
        mStart = value;
    else if(key == "end")
        mEnd = value;
}

//! Get a int parameter
int Range::getInt(string key)
{
    int result = 0;
    if(key == "start")
        result = mStart;
    else if(key == "end")
        result = mEnd;
    return result;
}
