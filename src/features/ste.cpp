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


#include "features/ste.h"

//! Run the algorithm and store the results
bool STEExtr::extract(int16_t* data, int size, int channels)
{
    if(size)
    {
        mSTE = 0;
        if(mSquare)
        {
            for(int i = 0; i < size; i++)
                mSTE += ((int)data[channels*i]) * ((int)data[channels*i]) / size;
        }
        else
        {
            for(int i = 0; i < size; i++)
                mSTE += fabs(data[channels*i]);
        }
    }
    return (size != 0);
}

//! Set a int parameter (available : "square", which is 0 or 1)
void STEExtr::setInt(string key, int value)
{
    if(key == "square")
        mSquare = value;
}

//! Get a int parameter (available : "square", which is 0 or 1)
int STEExtr::getInt(string key)
{
    int ret = 0;
    if(key == "square")
        ret = (mSquare ? 1 : 0);
    return ret;
}

