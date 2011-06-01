#ifndef INCLUDED_LSTERH
#define INCLUDED_LSTERH

//! Computes the High ZCR Ratio
class LSTERExtr;

#include "features/extractor.h"
#include "features/ste.h"

#define DEFAULT_STE_BOUND 0.5
#define DEFAULT_LSTER_CHUNK_SIZE 40

class LSTERExtr : public FeatureExtractor
{
    public:
        //! Default constructor
        LSTERExtr(int chunkSize = 0);

        //! Set the STE extractor (the previous one is returned)
        STEExtr* setSTEExtractor(STEExtr* extr);

        //! Run the algorithm and store the results
        bool extract(uint16_t* data, int size);

        //! Retrive the results (from the index). The values are usually between -1 and 1
        float value(int index = 0) { return mLSTER; };

        //! Get the number of available values
        int size() { return 1; }

        //! Set a float parameter (available : "bound", from 0 to 1)
        void setFloat(string key, float value);

        //! Set a int parameter (available : "chunkSize")
        void setInt(string key, int value);

        //! Get a float parameter (available : "bound")
        float getFloat(string key);

        //! Get a int parameter (available : "chunkSize")
        int getInt(string key);
    private:
        STEExtr *mSteExtr;
        float* mHistory;
        int mCurrentFrame;

        float mLSTER;
        float mBound;
        int mChunkSize;
};

#endif

