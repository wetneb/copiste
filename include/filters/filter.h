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


#ifndef INCLUDED_FILTERH
#define INCLUDED_FILTERH

#include "features/extractor.h"
#include <vector>

using namespace std;

//! Abstract class standing for a feature filter (counts the high / low values, computes the average…)
class Filter : public FeatureExtractor
{
    public:
        //! Default constructor : does nothing yet.
        Filter() : mExtr(0) { ; }

        //! Destructor (inherited from FeatureExtractor) \todo virtual ??
        ~Filter() { ; }

        //! Binds the filter to the output of an extractor (or another filter). Returns the old one.
        virtual FeatureExtractor* bind(FeatureExtractor* extr);

        /** \brief Extract data
         * This method should not be reimplemented by the user.
         * The given parameters are ignored : the data is drawn from the upstream extractor without
         * taking care of the raw data.
         */
        bool extract(uint16_t* data = 0, int size = 0);

        //! Do the actual computation on the features
        virtual void transform(vector<float> data) = 0;

    protected:
        FeatureExtractor* mExtr;
};

#endif
