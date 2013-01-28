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


#include "features/lster.h"

//! Default constructor
LSTERExtr::LSTERExtr(int chunkSize)
{
    mLSTER = 0;
    mBound = DEFAULT_STE_BOUND;
    mChunkSize = DEFAULT_LSTER_CHUNK_SIZE;
    mSteExtr = 0;

    // Fill the history with dummy data at first
    mHistory = new float[mChunkSize]; // deleted in the destructor
    clear();
}

//! Run the algorithm and store the results
bool LSTERExtr::extract(std::deque<int16_t> data, int size)
{
    if(mSteExtr == 0)
        mSteExtr = new STEExtr();

    if(size)
    {
        mSteExtr->extract(data, size);
        mHistory[mCurrentFrame] = mSteExtr->value();

        float average = 0;
        for(int i = 0; i < mChunkSize; i++)
            average += mHistory[i];
        average /= mChunkSize;
        float bound = mBound*average;

        unsigned int low = 0;
        for(int i = 1; i < mChunkSize; i++)
            if(mHistory[i] < bound)
                low++;

        mCurrentFrame = (mCurrentFrame + 1) % mChunkSize;

        mLSTER = (float)low / mChunkSize;
    }
    return size;
}

void LSTERExtr::clear()
{
    for(int i = 0; i < mChunkSize; i++)
        mHistory[i] = i%2;
    mLSTER = 0.5;
    mCurrentFrame = 0;
}

//! Set a float parameter (available : "bound")
void LSTERExtr::setFloat(string key, float value)
{
    if(key == "bound")
        mBound = value;
}

//! Set a int parameter (available : "chunkSize")
void LSTERExtr::setInt(string key, int value)
{
    if(key == "chunkSize")
        mChunkSize = value;
}

//! Get a float parameter (available : "bound")
float LSTERExtr::getFloat(string key)
{
    float ret = 0;
    if(key == "bound")
        ret = mBound;
    return ret;
}

//! Get a int parameter (available : "chunkSize")
int LSTERExtr::getInt(string key)
{
    int ret = 0;
    if(key == "chunkSize")
        ret = mChunkSize;
    return ret;
}

//! Set the STE extractor (the previous one is returned)
STEExtr* LSTERExtr::setSTEExtractor(STEExtr* extr)
{
    STEExtr* old = mSteExtr;
    mSteExtr = extr;
    return old;
}

LSTERExtr::~LSTERExtr()
{
    delete mHistory;
}
