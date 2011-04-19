#ifndef INCLUDED_SPECTRUMH
#define INCLUDED_SPECTRUMH

#include "features/extractor.h"

//! Retrives the spectrum using FFT (currently, the naive algorithm)
class SpectrumExtr : public FeatureExtractor
{
    public:
        //! Inits the FFT algorithm
        SpectrumExtr(int size = 0);

        //! Reallocate buffers based on the new size (plus butterfly computation)
        void reallocate();

        //! Cleans everything up
        ~SpectrumExtr();

        //! Run the algorithm
        bool extract(uint16_t* data, int size);

        //! Normalize the values (so that the values go from 0 to bound)
        void normalize(int bound);

        //! Retrieve the results (from the index). The values are usually between -1 and 1
        float value(int index);

        //! Retrieve the entire array
        uint16_t* spectrum();

        //! Get the number of available values
        int size();

        //! Get the highest lower power of two
        static int regularSize(int size);

        //! Get E(log2(x))
        static int log2(int x);

    private:
        uint16_t* mResults;
        int* mButterfly;
        int mSize;
        int mBound;
};

#endif
