#ifndef INCLUDED_ZCRH
#define INCLUDED_ZCRH

//! Computes the Zero Crossing Rate
class ZCRExtr;

#include "features/extractor.h"

class ZCRExtr : public FeatureExtractor
{
    public:
        //! Default constructor
        ZCRExtr(int chunkSize = 0) { mZCR = 0; }

        /**
         * \brief Run the algorithm and store the results.
         * If the first value of the data is the same as the previous one, the last value
         * will be used. This is just a matter of optimisation and the collisions aren't
         * a problem since they won't change basicly the ZCR variations or the HZCRR.
         */
        bool extract(uint16_t* data, int size);

        //! Retrive the results (from the index). The values are usually between -1 and 1
        float value(int index = 0) { return mZCR; };

        //! Get the number of available values
        int size() { return 1; }

        //! Set a float parameter (available : none)
        void setFloat(string key, float value) { ; }

        //! Set a int parameter (available : none)
        void setInt(string key, int value) { ; }

        //! Get a float parameter (available : none)
        float getFloat(string key) { return 0; }

        //! Get a int parameter (available : none)
        int getInt(string key) { return 0; }
    private:
        float mZCR;
        uint16_t mLastFirstValue;
};

#endif
