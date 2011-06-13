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
bool LSTERExtr::extract(uint16_t* data, int size)
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
