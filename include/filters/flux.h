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


#ifndef INCLUDED_FLUXH
#define INCLUDED_FLUXH

#include "filters/filter.h"

/*
 * Reference : A Robust Audio Classification and Segmentation Method
 * Lie Lu, Hao Jiang and HongJiang Zhang
 */

//! A filter for computing the amount of variations of a given feature
class FluxFilter : public Filter
{
    public:
        //! Compute the transformation : compute the "flux"
        void transform(vector<float> data);

        //! Get the number of available values (the same as in the original feature)
        int size();

        //! Return the asked value (indices are the same as in the original feature)
        float value(int index = 0);

        //! Lower bound for outputted values : 0
        float min() { return 0; }
        //! Higher bound for outputted values
        float max();

    private:
        vector<float> mLastVect;
        vector<float> mResult;
};

#endif
