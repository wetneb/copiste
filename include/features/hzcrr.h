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


#ifndef INCLUDED_HZCRRH
#define INCLUDED_HZCRRH

//! Computes the High ZCR Ratio
class HZCRRExtr;

#include "features/extractor.h"
#include "features/zcr.h"
#include "core/streamplayer.h" // for DEFAULT_AUDIO_CHUNK_SIZE 

#define DEFAULT_ZCR_BOUND 2.0
#define DEFAULT_HZCRR_CHUNKS_NUMBER (44100 / DEFAULT_AUDIO_CHUNK_SIZE)

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
        bool extract(std::deque<uint16_t> data, int size);

        //! Retrieve the results (from the index). The values are usually between -1 and 1
        float value(int index = 0) { return mHZCRR; };

        //! Get the number of available values
        int size() { return 1; }
        //! Lower bound on outputted values
        float min() { return 0; }
        //! Upper bound on outputted values
        float max() { return 1; }

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
