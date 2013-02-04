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


#ifndef INCLUDED_STEH
#define INCLUDED_STEH

class STEExtr;

#include "features/extractor.h"

#include <cmath>

//! Computes the Short Time Energy
class STEExtr : public FeatureExtractor
{
    public:
        //! Default constructor
        STEExtr(int chunkSize = 0) : mSquare(true) { mSTE = 0; }

        //! Run the algorithm and store the results
        bool extract(int16_t* data, int size, int channels);

        //! Retrive the results (from the index).
        float value(int index = 0) { return mSTE; };

        //! Get the number of available values
        int size() { return 1; }
        //! Lower bound on outputted values
        float min() { return 0; }
        //! Upper bound on outputted values (about 2^30)
        float max() { return ((unsigned int)(-1))*((unsigned int)(-1))/4; }

        //! Set a int parameter (available : "square", which is 0 or 1)
        void setInt(string key, int value);
        //! Get a int parameter (available : "square", which is 0 or 1)
        int getInt(string key);

    private:
        float mSTE;
        bool mSquare;
};

#endif
