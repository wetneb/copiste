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


#include "filters/range.h"

RangeFilter::RangeFilter()
{
    mStart = 0;
    mEnd = 1;
    mBlockSize = -1;
}

//! Do the actual computation on the features
void RangeFilter::transform(vector<float> data)
{
    mAverage.resize(size());
    for(int i = 0; i < size(); i++)
    {
        int blocksize = (mBlockSize == -1 ? data.size() : mBlockSize);
        int end = std::min(mEnd, std::min((int)data.size(), (i+1)*blocksize));
        for(int j = i*blocksize; j < end; j++)
                mAverage[i] += data[i];
        mAverage[i] /= (end - i*blocksize);
    }
}

//! Set a int parameter (available : start, end)
void RangeFilter::setInt(string key, int value)
{
    if(key == "start")
        mStart = value;
    else if(key == "end")
        mEnd = value;
    else if(key == "blocksize")
        mBlockSize = value;
}

//! Get a int parameter (available : start, end)
int RangeFilter::getInt(string key)
{
    int result = 0;
    if(key == "start")
        result = mStart;
    else if(key == "end")
        result = mEnd;
    else if(key == "blocksize")
        result = mBlockSize;
    return result;
}

int RangeFilter::size()
{
    int res = 0;
    if(mBlockSize == -1)
        res = 1;
    else if(mExtr->size() % mBlockSize)
        res = (std::min(mEnd, mExtr->size()) - mStart) / mBlockSize + 1;
    else
        res = (std::min(mEnd, mExtr->size()) - mStart) / mBlockSize;
    return res;
}

float RangeFilter::min()
{
    return (mExtr ? mExtr->min() : 0);
}

float RangeFilter::max()
{
    return (mExtr ? mExtr->max() : 1);
}

float RangeFilter::value(int index)
{
    float res = 0;
    if(index >= 0 && index < size())
        res = mAverage[index];
    return res;
}

