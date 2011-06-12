#ifndef INCLUDED_STEH
#define INCLUDED_STEH

//! Computes the Short Time Energy
class STEExtr;

#include "features/extractor.h"

#include <cmath>

class STEExtr : public FeatureExtractor
{
    public:
        //! Default constructor
        STEExtr(int chunkSize = 0) : mSquare(true) { mSTE = 0; }

        //! Run the algorithm and store the results
        bool extract(uint16_t* data, int size);

        //! Retrive the results (from the index). The values are usually between -1 and 1
        float value(int index = 0) { return mSTE; };

        //! Get the number of available values
        int size() { return 1; }

        //! Set a float parameter (available : none)
        void setFloat(string key, float value) { ; }

        //! Set a int parameter (available : "square", which is 0 or 1)
        void setInt(string key, int value);

        //! Get a float parameter (available : none)
        float getFloat(string key) { return 0; }

        //! Get a int parameter (available : "square", which is 0 or 1)
        int getInt(string key);
    private:
        float mSTE;
        bool mSquare;
};

#endif
