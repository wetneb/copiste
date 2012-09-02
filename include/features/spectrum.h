/*
 * This file is part of Copiste.
 *
 * Copiste is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 *  Copiste is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Copiste.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef INCLUDED_SPECTRUMH
#define INCLUDED_SPECTRUMH

#include "features/extractor.h"

/**
 * \class SpectrumExtr
 * \brief Retrives the spectrum using FFT
 */
class SpectrumExtr : public FeatureExtractor
{
    public:
        /// Memory management
        // The algorithm automaticly adapts its data (output, butterfly, window)
        // to the size of the data it gets.
        // In order to be more efficient in the actual computing, we try to compute
        // everything before.

        //! Inits the FFT algorithm
        SpectrumExtr(int chunkSize = 0);
        //! Reallocate buffers based on the new size (plus butterfly computation)
        void reallocate();
        //! Cleans everything up
        ~SpectrumExtr();

        /// Algorithm
        // It is the standard FFT algorithm : the recursive design has been translated into
        // an iterative process. The output is normalized as the values can get quite high
        // rates.

        //! Run the algorithm
        bool extract(std::deque<uint16_t> data, int size);
        //! Normalize the values (so that the values go from 0 to bound)
        void normalize(int bound);

    private:
        /// Windows
        // The windows are intended to minimize the lack of precision of the actual computation
        // of the Fourier Transform. That's a well-known phenomon : when you try to compute
        // the spectrum of a perfect sinus wave on a finite window, you don't get a pefect dirac
        // as expected, but a large wave with some noise.

        //! Rectangular
        static double rectangularWin(int i, int maxSize);
        //! Linear
        static double triangularWin(int i, int maxSize);
        //! Hamming
        static double hammingWin(int i, int maxSize);
        //! Blackman - Harris
        static double blackmanHarrisWin(int i, int maxSize);

        //! Updates the window cache
        void createWindowCache();

    public:
        enum FTWindow
        { WINDOW_RECTANGULAR,
          WINDOW_TRIANGULAR,
          WINDOW_HAMMING,
          WINDOW_BH };
        typedef enum FTWindow FTWindow;

        /// Accessors
        // These functions can be used to retrive the result of the computation.

        //! Retrieve the results (from the index). 
        float value(int index);
        //! Retrieve the entire array
        uint16_t* spectrum();
        //! Get the number of available values
        int size();
        //! Lower bound on outputted values
        float min() { return 0; }
        //! Upper bound on outputted values
        float max() { return (uint16_t)(-1); }

        //! Set a double parameter (available : none)
        void setFloat(string key, float value) { ; }
        //! Set a int parameter (available : "bound")
        void setInt(string key, int value);
        //! Get a double parameter (available : none)
        float getFloat(string key) { return 0; }
        //! Get a int parameter (available : "bound")
        int getInt(string key);
        //! Change the window (the default is WINDOW_RECTANGULAR)
        void setWindow(FTWindow win);

    private:
        /// Utilities
        // The size of the data sent to the algorithm must be a power of 2.

        //! Get the highest lower power of two
        static int regularSize(int size);
        //! Get E(log2(x))
        static int log2(int x);
        //! Get 2^n
        static int pow2(int n);

    public:
        /// Watching thread
        void watch();

    private:
        uint16_t* mResults;
        int* mButterfly;
        double* mWindowCache;
        FTWindow mCurrentWindow;
        int mSize;
        int mBound;
};

#endif
