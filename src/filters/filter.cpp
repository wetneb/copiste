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


#include "filters/filter.h"

FeatureExtractor* Filter::bind(FeatureExtractor* extr)
{
    FeatureExtractor* old = mExtr;
    mExtr = extr;
    parentChanged();
    return old;
}

//! Extract data
bool Filter::extract(int16_t* dat, int size, int channels)
{
    if(mExtr)
    {
        vector<float> data;
        data.resize(mExtr->size());
        // TODO, this is ugly
        for(unsigned int i = 0; i < data.size(); i++)
            data[i] = mExtr->value(i);
        transform(data);
    }

    return true;
}
