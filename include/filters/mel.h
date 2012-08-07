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


#ifndef INCLUDED_MELH
#define INCLUDED_MELH

#include "filters/filter.h"

//! A filter displaying a feature on a logarithmic scale
class MelFilter : public Filter
{
    public:
        //! Default constructor : does nothing
        MelFilter();

        //! Destructor : does nothing 
        ~MelFilter() { ; }

        //! Run the algorithm and store the results
        void transform(vector<float> data);

        //! Retrieve the results (from the index).
        float value(int index = 0);

        //! Get the number of available values : computed from the size of the parent
        int size();

        //! Set a float parameter (available : base)
        void setFloat(string key, float value);
        //! Get a int parameter  (available : base)
        float getFloat(string key);

   protected:
        //! Update the sizes
        void parentChanged();

    private:
	vector<float> mResults;
        int mSize;
	float mBase;
};

#endif

