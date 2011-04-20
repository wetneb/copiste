#include "features/lster.h"

//! Default constructor
LSTERExtr::LSTERExtr(int chunkSize)
{
    mLSTER = 0;
    mBound = DEFAULT_STE_BOUND;
    mChunkSize = DEFAULT_LSTER_CHUNK_SIZE;
}

//! Run the algorithm and store the results
bool LSTERExtr::extract(uint16_t* data, int size)
{
    if(size)
    {
        int lowCount = 0;
        uint16_t zero = (-1);
        zero >>= 1;

        // Compute the average energy
        int average = 0;
        for(int i = 0; i < size; ++i)
            average += ((int)data[i] - zero) * ((int)data[i] - zero) / size;

        // Count the low STE
        int upperBound = mBound * average;
        for(int i = 0; i < size; ++i)
        {
            if(((int)data[i] - zero) * ((int)data[i] - zero) < upperBound)
                lowCount++;
        }

        mLSTER = lowCount / size;
    }
    return size;
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


