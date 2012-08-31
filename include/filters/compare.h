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


#ifndef INCLUDED_COMPAREH
#define INCLUDED_COMPAREH

#include <filters/filter.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <fstream>

using namespace boost::numeric;

//! Compares the values of a feature to a set of known patterns
class CompareFilter : public Filter
{
    public:
        CompareFilter();

        //! Load a profile of patterns
        bool loadProfile(string filename);

        //! Compute the differences
        void transform(vector<float> data);

        //! Number of results : the number of patterns compared
        int size() { return mPatterns.size1(); }
        //! Retrieve the results
        float value(int idx);
        
        //! Overloaded from FeatureExtractor
        void setString(string key, string value);
        //! Overloaded from FeatureExtractor
        string getString(string key);
       
        //! Get the minimum value outputted
        float min();
        //! Get the maximum value outputted
        float max();
    private:
        ublas::matrix<int> mPatterns;
        vector<int> mEnergy;
        string mFilename;
        vector<float> mResult;        
};

#endif

