#include "filters/average.h"

//! Run the algorithm and store the results
void AverageFilter::transform(vector<float> data)
{
    if(mHistory == 0)
    {
        mHistory = new float*[mSize]; //! TODO : missing delete
        for(int i = 0; i < mSize; i++)
        {
            mHistory[i] = new float[mDimension]; //! TODO : missing delete
            for(int j = 0; j < mDimension; j++)
                mHistory[i][j] = 0;
        }

        if(mAverage != 0)
            delete mAverage;

        mAverage = new float[mDimension];
        for(int i = 0; i < mDimension; i++)
            mAverage[i] = 0;
    }

    for(unsigned int i = 0; i < data.size() && i < (unsigned int)mDimension; i++)
    {
        mAverage[i] += (data[i] - mHistory[mCurrentIndex][i]) / mSize;
        mHistory[mCurrentIndex][i] = data[i];
    }

    mCurrentIndex = (mCurrentIndex+1) % mSize;
}

//! Retrive the results (from the index). The values are usually between -1 and 1
float AverageFilter::value(int index)
{
    if(mAverage)
        return mAverage[index];
    return 0;
}

//! Get the number of available values
int AverageFilter::size()
{
    //if(mExtr != 0 && mExtr->size() != mDimension)
    return mDimension;
}

//! Set a int parameter (available : size)
void AverageFilter::setInt(string key, int value)
{
    if(key == "size")
        mSize = value;
}

//! Get a int parameter (available : size)
int AverageFilter::getInt(string key)
{
    int result = 0;
    if(key == "size")
        result = mSize;
    return result;
}

FeatureExtractor* AverageFilter::bind(FeatureExtractor* extr)
{
    FeatureExtractor* old = mExtr;
    mExtr = extr;
    mDimension = mExtr->size();
    return old;
}

