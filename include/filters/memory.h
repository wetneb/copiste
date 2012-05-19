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


#ifndef INCLUDED_MEMORYH
#define INCLUDED_MEMORYH

#include "filters/filter.h"

//! A class for keeping track of the last n values of features
class MemoryFilter : public Filter
{
    public:
	//! Constructor
	MemoryFilter();

        //! Do the actual computation on the features
        void transform(vector<float> data);

        //! Returns the number of available features : n*m (where m is the number of input features)
        int size() { return mN*mM; }
        //! Lower bound for outputted values
        float min();
        //! Higher bound for outputted values
        float max();

        //! Get the result (index = i*n + j where i is the feature and j the frame)
        float value(int index = 0);

        //! Set a int parameter : "depth", the n parameter (depth of the memory)
        void setInt(string key, int value);
        //! Get a int parameter : "depth"
        int getInt(string key);

   protected:
        //! Updates the sizes
        void parentChanged();

    private:
    	//! The "depth" of the memory
        unsigned int mN;
	    //! The number of features
	    unsigned int mM;
	    //! The memory
	    vector< vector<float> > mMem;
	    //! Current index where to fill data
	    unsigned int mCurr;
};

#endif
