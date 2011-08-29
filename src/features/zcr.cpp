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


#include "features/zcr.h"

//! Run the algorithm and store the results
bool ZCRExtr::extract(uint16_t* data, int size)
{
    if(size && (data[0] != mLastFirstValue))
    {
        int crossingsCount = 0;
        uint16_t zero = (-1);
        zero >>= 1;
        bool higher = data[0] > zero;

        for(int i = 1; i < size; i++)
        {
            if(higher != (data[i] > zero))
            {
                higher = !higher;
                crossingsCount++;
            }
        }

        mZCR = (float)crossingsCount / size;
        mLastFirstValue = data[0];
    }
    return size;
}

