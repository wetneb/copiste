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


#ifndef INCLUDED_HIGHLOWRATIOH
#define INCLUDED_HIGHLOWRATIOH

#include "filters/filter.h"

#define DEFAULT_HIGH_LOW_WINDOW_SIZE 100

/**
 * \brief Counts the number of high or low values on a given feature
 * It computes the average and compares each value to the average multiplied by "bound",
 * the float can be set in the XML file.
 * The user can also decide if the number of values higher or lower should be counted.
 */
class HighLowRatioFilter : public Filter
{
    public:
        //! Constructor : does nothing by default
        HighLowRatioFilter() : mBound(1),
                               mHigh(true),
                               mWindowSize(DEFAULT_HIGH_LOW_WINDOW_SIZE),
                               mCurrentIndex(0)
                               { ; }

        //! Cleans the memory
        ~HighLowRatioFilter();

        //! Extracts the feature by counting the high/low values
        void transform(vector<float> data);

        //! Retrive the results
        float value(int index = 0) { return mResults[index] / mWindowSize; }

        //! Set a float parameter (available : bound)
        void setFloat(string key, float value);

        //! Set a int parameter (available : high, windowSize)
        void setInt(string key, int value);

        //! Get a float parameter (available : bound)
        float getFloat(string key);

        //! Get a int parameter (available : high, windowSize)
        virtual int getInt(string key);

        //! Returns the size of the results (which is the size of the upstream feature)
        int size() { return mResults.size(); }

    private:
        void reallocate(unsigned int newSize);

        float mBound;
        bool mHigh;
        int mWindowSize;
        vector<float> mResults;
        vector<float> mAverage;
        vector<float*> mMemory;
        unsigned int mCurrentIndex;
};

#endif
