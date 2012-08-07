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

#include "filters/mel.h"

#include <cmath> // for log()

MelFilter::MelFilter() : Filter(),
			mSize(0),
			mBase(2)
{
	;
}

void MelFilter::transform(vector<float> data)
{
	int count = 0;
	float sum = 0, eexp = 1.0, exponent = 0;
	for(unsigned int i = 0; i < data.size(); i++)
	{
		if((float)i >= eexp && exponent < mResults.size())
		{
			mResults[exponent] = sum / (std::max(count, 1));
			exponent++;
			sum = 0;
			eexp *= mBase;
		}

		sum += data[i];
	}
}

float MelFilter::value(int index)
{
	return (index < (int)mResults.size()) ? mResults[index] : 0;
}

void MelFilter::setFloat(string key, float value)
{
	if(key == "base")
	{
		mBase = value;
		parentChanged(); // update the size
	}
}

float MelFilter::getFloat(string key)
{
	float result = 0;
	if(key == "base")
		result = mBase;
	return result;
}

void MelFilter::parentChanged()
{
	if(mExtr == 0 || mExtr->size() == 0)
		mSize = 0;
	else
		mSize = log(mExtr->size()) / log(mBase) + 1;
	mResults.resize(mSize);
	std::cout << "New size : " << mSize << std::endl;
}

int MelFilter::size()
{
	return mSize;
}
		
