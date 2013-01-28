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


#ifndef INCLUDED_LSTERH
#define INCLUDED_LSTERH

class LSTERExtr;

#include "features/extractor.h"
#include "features/ste.h"

const float DEFAULT_STE_BOUND = 0.5;
const int DEFAULT_LSTER_CHUNK_SIZE = 40;

/**
 * \class LSTERExtr
 * \brief Extracts the Low STE Ratio (STE stands for Short Time Energy)
 */
class LSTERExtr : public FeatureExtractor
{
    public:
        //! Default constructor
        LSTERExtr(int chunkSize = 0);

        //! Destructor
        ~LSTERExtr();

        //! Set the STE extractor (the previous one is returned)
        STEExtr* setSTEExtractor(STEExtr* extr);

        //! Run the algorithm and store the results
        bool extract(std::deque<int16_t> data, int size);

        //! Retrieve the results (from the index).
        float value(int index = 0) { return mLSTER; };

        //! Get the number of available values
        int size() { return 1; }
        //! Lower bound on the outputted values
        float min() { return 0; }
        //! Upper bound on the outputted values
        float max() { return 1; }

        //! Set a float parameter (available : "bound", from 0 to 1)
        void setFloat(string key, float value);
        //! Set a int parameter (available : "chunkSize")
        void setInt(string key, int value);
        //! Get a float parameter (available : "bound")
        float getFloat(string key);
        //! Get a int parameter (available : "chunkSize")
        int getInt(string key);

        //! Clear the extractor (forget the previous results)
        void clear();
    private:
        STEExtr *mSteExtr;
        float* mHistory;
        int mCurrentFrame;

        float mLSTER;
        float mBound;
        int mChunkSize;
};

#endif

