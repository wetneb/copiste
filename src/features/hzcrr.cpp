#include "features/hzcrr.h"

//! Default constructor
HZCRRExtr::HZCRRExtr(int chunkSize)
{
    mHZCRR = 0;
    mBound = DEFAULT_ZCR_BOUND;
    mChunkSize = DEFAULT_HZCRR_CHUNK_SIZE;
}

//! Run the algorithm and store the results
bool HZCRRExtr::extract(uint16_t* data, int size)
{
    if(size)
    {
        int highCount = 0;
        for(int i = 0; i < size; i += mChunkSize)
        {
            mZcrExtr.extract(data + i*sizeof(uint16_t), mChunkSize);
            if(mZcrExtr.value() >= mHZCRR)
                highCount++;
        }

        mHZCRR = (float)highCount * mChunkSize / size;
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
    if(key == "chunkSize")
        mChunkSize = value;
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
    if(key == "chunkSize")
        ret = mChunkSize;
    return ret;
}

