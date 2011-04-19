#ifndef INCLUDED_ZCRH
#define INCLUDED_ZCRH

//! Computes the Zero Crossing Rate
class ZCRExtr;

#include "features/extractor.h"

class ZCRExtr : public FeatureExtractor
{
    public:
        //! Default constructor
        ZCRExtr() { mZCR = 0; }

        //! Run the algorithm and store the results
        bool extract(uint16_t* data, int size);

        //! Retrive the results (from the index). The values are usually between -1 and 1
        float value(int index = 0) { return mZCR; };

        //! Get the number of available values
        int size() { return 1; }
    private:
        float mZCR;
};

#endif
