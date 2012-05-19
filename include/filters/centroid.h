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


#ifndef INCLUDED_CENTROIDH
#define INCLUDED_CENTROIDH

#include "filters/filter.h"

//! A filter for computing the centroid of a feature (i.e. the average indice). See code.
class CentroidFilter : public Filter
{
    public:
        //! Compute the feature
        void transform(vector<float> data);

        //! Returns the result (available : "centroid")
        float value(int index = 0) { return mCentroid; }

        //! Size is 1
        int size() { return 1; }
        //! Lower bound for outputted values : 0
        float min() { return 0; } 
        //! Higher bound for outputted values : the same as of the parent
        float max();

    private:
        float mCentroid;
};

#endif
