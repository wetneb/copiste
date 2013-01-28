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


#ifndef INCLUDED_ZCRH
#define INCLUDED_ZCRH

class ZCRExtr;

#include "features/extractor.h"

//! Computes the Zero Crossing Rate
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
        bool extract(std::deque<int16_t> data, int size);

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
        int16_t mLastFirstValue;
};

#endif
