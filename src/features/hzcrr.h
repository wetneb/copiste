#ifndef INCLUDED_HZCRRH
#define INCLUDED_HZCRRH

//! Computes the High ZCR Ratio
class HZCRRExtr;

#include "features/extractor.h"
#include "features/zcr.h"

#define DEFAULT_ZCR_BOUND 0.5
#define DEFAULT_HZCRR_CHUNK_SIZE 64

/**
 * \brief Extracts the High ZCR Ratio (ZCR stands for Zero Crossing Rate)
 *
 * This value is defined by Lie Lu, Hong-Jiang Zhang by the percentage of
 * the frames that have ZCR above 1.5 the average on an 1 sec window.
 * TODO : Fix the implementation.
 * TODO : Allow it to use the ZCR values that have already been computed.
 */
class HZCRRExtr : public FeatureExtractor
{
    public:
        //! Default constructor
        HZCRRExtr(int chunkSize = 0);

        //! Run the algorithm and store the results
        bool extract(uint16_t* data, int size);

        //! Retrive the results (from the index). The values are usually between -1 and 1
        float value(int index = 0) { return mHZCRR; };

        //! Get the number of available values
        int size() { return 1; }

        //! Set a float parameter (available : "bound")
        void setFloat(string key, float value);

        //! Set a int parameter (available : "chunkSize")
        void setInt(string key, int value);

        //! Get a float parameter (available : "bound")
        float getFloat(string key);

        //! Get a int parameter (available : "chunkSize")
        int getInt(string key);

    private:
        ZCRExtr mZcrExtr;
        float mHZCRR;
        float mBound;
        int mChunkSize;
};

#endif
