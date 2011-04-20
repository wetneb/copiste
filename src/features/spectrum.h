#ifndef INCLUDED_SPECTRUMH
#define INCLUDED_SPECTRUMH

#include "features/extractor.h"

//! Retrives the spectrum using FFT (currently, the naive algorithm)
class SpectrumExtr : public FeatureExtractor
{
    public:
        /// Memory management

        //! Inits the FFT algorithm
        SpectrumExtr(int chunkSize = 0);
        //! Reallocate buffers based on the new size (plus butterfly computation)
        void reallocate();
        //! Cleans everything up
        ~SpectrumExtr();

        /// Algorithm

        //! Run the algorithm
        bool extract(uint16_t* data, int size);
        //! Normalize the values (so that the values go from 0 to bound)
        void normalize(int bound);

        /// Accessors

        //! Retrieve the results (from the index). The values are usually between -1 and 1
        float value(int index);
        //! Retrieve the entire array (TODO : delete me)
        uint16_t* spectrum();
        //! Get the number of available values
        int size();
        //! Set a float parameter (available : none)
        void setFloat(string key, float value) { ; }
        //! Set a int parameter (available : "bound")
        void setInt(string key, int value);
        //! Get a float parameter (available : none)
        float getFloat(string key) { return 0; }
        //! Get a int parameter (available : "bound")
        int getInt(string key);

        /// Utilities

        //! Get the highest lower power of two
        static int regularSize(int size);
        //! Get E(log2(x))
        static int log2(int x);

        /// Watching thread
        void watch();

    private:
        uint16_t* mResults;
        int* mButterfly;
        int mSize;
        int mBound;
};

#endif
