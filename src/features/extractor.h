#ifndef INCLUDED_EXTRACTORH
#define INCLUDED_EXTRACTORH

#include <vlc/vlc.h>

//! An abstract class, used to compute features from the data
class FeatureExtractor
{
    public:
        //! Run the algorithm and store the results
        virtual bool extract(uint16_t* data, int size) = 0;

        //! Retrive the results (from the index). The values are usually between -1 and 1
        virtual float value(int index = 0) = 0;

        //! Get the number of available values
        virtual int size() = 0;
};


#endif
