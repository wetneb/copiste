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


#include "filters/memory.h"

//! Constructior
MemoryFilter::MemoryFilter() : Filter()
{
	mN = 0;
	mM = 1;
	mCurr = 0;
}

//! Do the actual computation on the features
void MemoryFilter::transform(vector<float> data)
{

	if(mM == data.size())
	{
		mMem[mCurr] = data;
		mCurr = (mCurr + 1) % mN;
	}
}

float MemoryFilter::value(int index)
{
	unsigned int i = index / mN, j = index % mN;
	float result = 0;
	if(0 <= i && i < mMem.size())
		result = mMem[(mCurr - j) % mN][i];
    return result;	
}

void MemoryFilter::parentChanged()
{
    mM = mExtr->size();
	for(unsigned int i = 0; i < mN; i++)
		mMem[i].resize(mM, 0);
}

//! Set a int parameter (available : start, end)
void MemoryFilter::setInt(string key, int value)
{
    if(key == "depth") // don't update if the filter has been running
    {
        mN = value;
        mMem.resize(mN);
        for(unsigned int i = 0; i < mN; i++)
            mMem[i].resize(mM);
        mCurr = mCurr % mN;
    }
}

//! Get a int parameter (available : start, end)
int MemoryFilter::getInt(string key)
{
    int result = 0;
    if(key == "depth")
        result = mN;
    return result;
}
