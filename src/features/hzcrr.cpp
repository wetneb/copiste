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


#include "features/hzcrr.h"

//! Default constructor
HZCRRExtr::HZCRRExtr(int chunkSize)
{
    mZcrExtr = 0;
    mHZCRR = 0;
    mBound = DEFAULT_ZCR_BOUND;
    mChunksNumber = DEFAULT_HZCRR_CHUNKS_NUMBER;
    mCurrentFrame = 0;

    // Fill the history with dummy data at first
    mHistory = new float[mChunksNumber]; // deleted in the destructor
    for(int i = 0; i < mChunksNumber; i++)
        mHistory[i] = i%2;
    mHZCRR = 0.5;
}

//! Set the ZCR extractor that should be used by this extractor
ZCRExtr* HZCRRExtr::setZCRExtractor(ZCRExtr* extr)
{
    ZCRExtr* old = mZcrExtr;
    mZcrExtr = extr;
    return old;
}

/**
 * \brief Run the algorithm and store the results
 * Implementation :
 * A ZCR extractor is set, and can be used outside
 * the extractor, so that the values aren't computed
 * twice.
 *
 * A ZCR history is created : we compute the ZCR for
 * each frame and update the HZCRR. The history is a
 * circular buffer.
 */
bool HZCRRExtr::extract(int16_t* data, int size, int channels)
{
    // If no external ZCR extractor has been set, use a new one
    if(mZcrExtr == 0)
        mZcrExtr = new ZCRExtr();

    if(size)
    {
        mZcrExtr->extract(data, size, channels);
        mHistory[mCurrentFrame] = mZcrExtr->value();

        float average = 0;
        for(int i = 0; i < mChunksNumber; i++)
            average += mHistory[i];
        average /= mChunksNumber;
        float bound = mBound*average;

        unsigned int crossings = 0;
        //bool up = (mHistory[0] > bound);
        for(int i = 1; i < mChunksNumber; i++)
            if(mHistory[i] > bound)
            {
                crossings++;
                //up = !up;
            }

        mCurrentFrame = (mCurrentFrame + 1) % mChunksNumber;

        mHZCRR = (float)crossings / mChunksNumber;
    }
    return size;
}

//! Set a float parameter (available : "bound")
void HZCRRExtr::setFloat(string key, float value)
{
    if(key == "bound")
        mBound = value;
}

//! Set a int parameter (available : "chunkSize")
void HZCRRExtr::setInt(string key, int value)
{
    if(key == "chunksNumber")
    {
        mChunksNumber = value;
        delete mHistory;
        mHistory = new float[value];
    }
}

//! Get a float parameter (available : "bound")
float HZCRRExtr::getFloat(string key)
{
    float ret = 0;
    if(key == "bound")
        ret = mBound;
    return ret;
}

//! Get a int parameter (available : "chunkSize")
int HZCRRExtr::getInt(string key)
{
    int ret = 0;
    if(key == "chunksNumber")
        ret = mChunksNumber;
    return ret;
}

//! Destructor.
HZCRRExtr::~HZCRRExtr()
{
    delete mHistory;
}


