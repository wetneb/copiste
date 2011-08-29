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


#include "filters/centroid.h"

void CentroidFilter::transform(vector<float> data)
{
    float weightsSum = 0;
    float mean = 0;

    for(unsigned int i = 0; i < data.size(); i++)
    {
        weightsSum += data[i];
        mean += data[i]*(i+1);
    }

    mCentroid = mean / weightsSum;
}
