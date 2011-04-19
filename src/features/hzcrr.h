#ifndef INCLUDED_HZCRRH
#define INCLUDED_HZCRRH

//! Computes the High ZCR Ratio
class HZCRRExtr;

#include "features/extractor.h"
#include "features/zcr.h"

#define DEFAULT_ZCR_BOUND 0.7
#define DEFAULT_HZCRR_CHUNK_SIZE 64

class HZCRRExtr
{
    public:
        //! Default constructor
        HZCRRExtr();

        //! Run the algorithm and store the results
        bool extract(uint16_t* data, int size);

        //! Retrive the results (from the index). The values are usually between -1 and 1
        float value(int index = 0) { return mHZCRR; };

        //! Get the number of available values
        int size() { return 1; }

        //! Set rate bound
        void setBound(float bound) { mBound = bound; }

        //! Get rate bound
        float bound() { return mBound; }

        //! Set chunk size
        void setChunkSize(int size) { mChunkSize = size; }

        //! Get chunk size
        int chunkSize() { return mChunkSize; }
    private:
        ZCRExtr mZcrExtr;
        float mHZCRR;
        float mBound;
        int mChunkSize;
};

#endif
