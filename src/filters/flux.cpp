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


#include "filters/flux.h"

#include <cmath>

//! Compute the transformation : compute the "flux"
void FluxFilter::transform(vector<float> data)
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
int FluxFilter::size()
{
    int result = 0;
    if(mExtr)
        result = mExtr->size();
    if(result != (int)mResult.size())
        mResult.resize(result);
    return result;
}

//! Return the asked value (indices are the same as in the original feature)
float FluxFilter::value(int index)
{
    float result = 0;
    if((unsigned int)index < mResult.size())
        result = mResult[index];
    return result;
}
