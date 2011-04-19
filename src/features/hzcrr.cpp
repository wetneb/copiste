#include "features/hzcrr.h"

//! Default constructor
HZCRRExtr::HZCRRExtr()
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

