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


#ifndef INCLUDED_AVERAGEH
#define INCLUDED_AVERAGEH

#include "filters/filter.h"

//! A filter for computing the temporal average of a value
class AverageFilter : public Filter
{
    public:
        //! Default constructor : does nothing
        AverageFilter() : Filter(), mHistory(0), mAverage(0), mSize(0), mDimension(0), mCurrentIndex(0) { ; }

        //! Destructor : cleans the memory
        ~AverageFilter();

        //! Run the algorithm and store the results
        void transform(vector<float> data);

        //! Retrieve the results (from the index).
        float value(int index = 0);
        //! Lower bound for outputted values (same as the parent)
        float min();
        //! Higher bound for outputted values (same as the parent)
        float max();

        //! Get the number of available values
        int size();

        //! Set a int parameter (available : size)
        void setInt(string key, int value);
        //! Get a int parameter  (available : size)
        int getInt(string key);

   protected:
        //! Update the sizes
        void parentChanged();

    private:
        float** mHistory;
        float* mAverage;
        int mSize;
        int mDimension;
        int mCurrentIndex;
};

#endif
