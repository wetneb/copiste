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

//! Do the actual computation on the features
void RangeFilter::transform(vector<float> data)
{
    float sum = 0;
    for(int i = std::max(mStart, 0); i < std::min((int)data.size(), mEnd); i++)
        sum += data[i];
    mAverage = sum / (mEnd - mStart);
}

//! Set a int parameter (available : start, end)
void RangeFilter::setInt(string key, int value)
{
    if(key == "start")
        mStart = value;
    else if(key == "end")
        mEnd = value;
}

//! Get a int parameter (available : start, end)
int RangeFilter::getInt(string key)
{
    int result = 0;
    if(key == "start")
        result = mStart;
    else if(key == "end")
        result = mEnd;
    return result;
}

float RangeFilter::min()
{
    return (mExtr ? mExtr->min() : 0);
}

float RangeFilter::max()
{
    return (mExtr ? mExtr->max() : 1);
}

