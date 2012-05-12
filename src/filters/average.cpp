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


#include "filters/average.h"

//! Run the algorithm and store the results
void AverageFilter::transform(vector<float> data)
{
    if(mHistory == 0)
    {
        mHistory = new float*[mSize];
        for(int i = 0; i < mSize; i++)
        {
            mHistory[i] = new float[mDimension];
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

AverageFilter::~AverageFilter()
{
    if(mHistory != 0)
    {
        for(int i = 0; i < mSize; i++)
            delete mHistory[i];

        delete mHistory;
    }

    if(mAverage != 0)
        delete mAverage;
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

void AverageFilter::parentChanged()
{
    mDimension = mExtr->size();
}

