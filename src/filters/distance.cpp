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

#include "filters/distance.h"

DistanceFilter::DistanceFilter() : Filter()
{
    ;
}

bool DistanceFilter::loadProfile(string filename)
{
    std::ifstream ifs(filename.c_str());
    bool status = false;

    if(!ifs.is_open())
       std::cerr << "Unable to open the file " << filename << std::endl;
    else
    {
        try
        {
            boost::archive::text_iarchive ar(ifs);
            ar & mPattern;
            status = true;
        }
        catch(boost::archive::archive_exception ex)
        {
            std::cerr << "Error while loading the compare profile from the file " << filename
                << " : " << ex.what() << std::endl;
        }
    }
    return status;
}

void DistanceFilter::transform(vector<float> data)
{
    if(data.size() != mPattern.size())
        std::cerr << "Warning, profile and stream sizes don't match : " << mPattern.size() << " vs " << data.size() << " (DistanceFilter)" << std::endl;
    else
    {
       float min = mExtr->min(), max = mExtr->max(); 
       mResult.resize(mPattern.size());
       for(unsigned int i = 0; i < mPattern.size(); i++)
       {
           mResult[i] = abs(mPattern(i) - (data[i] - min)*255.0/(max - min));
       }
    }
}

float DistanceFilter::value(int idx)
{
    float res = 0;
    if(idx >= 0 && idx < (int)mResult.size())
        res = mResult[idx];
    return res;
}

void DistanceFilter::setString(string key, string value)
{
    if(key == "profile")
        loadProfile(value);
}

string DistanceFilter::getString(string key)
{
    string result = "";
    if(key == "profile")
        result = mFilename;
    return result;
}

float DistanceFilter::min()
{
    return 0;
}

float DistanceFilter::max()
{
    return 255;
}

