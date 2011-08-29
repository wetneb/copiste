#ifndef INCLUDED_HZCRRH
#define INCLUDED_HZCRRH

//! Computes the High ZCR Ratio
class HZCRRExtr;

#include "features/extractor.h"
#include "features/zcr.h"
#include "core/streamplayer.h" // for AUDIO_CHUNK_SIZE

#define DEFAULT_ZCR_BOUND 2.0
#define DEFAULT_HZCRR_CHUNKS_NUMBER (44100 / AUDIO_CHUNK_SIZE)

/**
 * \class HZCRRExtr
 * \brief Extracts the High ZCR Ratio (ZCR stands for Zero Crossing Rate)
 *
 * This value is defined by Lie Lu, Hong-Jiang Zhang by the percentage of
 * the frames that have ZCR above 1.5 the average on an 1 sec window.
 */
class HZCRRExtr : public FeatureExtractor
{
    public:
        //! Default constructor
        HZCRRExtr(int chunkSize = 0);

        //! Destructor
        ~HZCRRExtr();

        //! Set the ZCR extractor (the previous one is returned)
        ZCRExtr* setZCRExtractor(ZCRExtr* extr);

        //! Run the algorithm and store the results
        bool extract(uint16_t* data, int size);

        //! Retrive the results (from the index). The values are usually between -1 and 1
        float value(int index = 0) { return mHZCRR; };

        //! Get the number of available values
        int size() { return 1; }

        //! Set a float parameter (available : "bound")
        void setFloat(string key, float value);

        //! Set a int parameter (available : "chunksNumber")
        void setInt(string key, int value);

        //! Get a float parameter (available : "bound")
        float getFloat(string key);

        //! Get a int parameter (available : "chunksNumber")
        int getInt(string key);

    private:
        ZCRExtr* mZcrExtr;
        float* mHistory;
        int mCurrentFrame;

        float mHZCRR;
        float mBound;
        int mChunksNumber;
};

#endif
