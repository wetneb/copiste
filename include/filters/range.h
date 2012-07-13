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


#ifndef INCLUDED_RANGEH
#define INCLUDED_RANGEH

#include "filters/filter.h"

#include <vector>

//! A class for computing the average between a set of features (usually for the spectrum)
class RangeFilter : public Filter
{
    public:
        RangeFilter();

        //! Do the actual computation on the features
        void transform(vector<float> data);

        //! Returns the number of available features : parent->size() / blocksize (+ 1)
        int size();
        //! Lower bound : the same as parent's
        float min();
        //! Higher bound : the same as parent's
        float max();

        //! Get the result (the index points to the desired block)
        float value(int index); 

        //! Set a int parameter (available : start, end, blocksize)
        void setInt(string key, int value);
        //! Get a int parameter (available : start, end, blocksize)
        int getInt(string key);

    private:
        int mStart;
        int mEnd;
        int mBlockSize;
        std::vector<float> mAverage;
};

#endif

