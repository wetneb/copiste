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

#include <filters/compare.h>

CompareFilter::CompareFilter() : Filter()
{
    ;
}

bool CompareFilter::loadProfile(string filename)
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
            ar & mPatterns;
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

void CompareFilter::transform(vector<float> data)
{
    if(data.size() != mPatterns.size2())
        std::cerr << "Warning, profile and stream sizes don't match : " << mPatterns.size2() << " vs " << data.size() << " (CompareFilter)" << std::endl;
    
    mResult.resize(mPatterns.size1());
    float tmp = 0;
    for(unsigned int i = 0; i < mPatterns.size1(); i++)
    {
        float sum = 0;
        for(unsigned int j = 0; j < mPatterns.size2(); j++)
        {
            tmp = (mPatterns(i,j) - data[j]);
            sum += (tmp * tmp);
        }
        mResult[i] = sum / mPatterns.size2();
    }
}

float CompareFilter::value(int idx)
{
    float res = 0;
    if(idx >= 0 && idx < (int)mResult.size())
        res = mResult[idx];
    return res;
}

void CompareFilter::setString(string key, string value)
{
    if(key == "profile")
        loadProfile(value);
}

string CompareFilter::getString(string key)
{
    string result = "";
    if(key == "profile")
        result = mFilename;
    return result;
}

float CompareFilter::min()
{
    return 0;
}

float CompareFilter::max()
{
    return (mExtr ? (mExtr->max() - mExtr->min()) * (mExtr->max() - mExtr->min()) : 1);
}

