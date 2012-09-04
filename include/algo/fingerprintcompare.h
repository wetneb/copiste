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


#ifndef INCLUDED_FINGERPRINTCOMPAREH
#define INCLUDED_FINGERPRINTCOMPAREH

#include "algo/abstractclassifier.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <fstream>
#include <QtXml>

using namespace boost::numeric;

double DEFAULT_PATTERN_COMPARE_THRESHOLD = 0.05;

//! Compares the values of a feature to a set of known patterns
class FingerprintCompare : public AbstractClassifier
{
    public:
        FingerprintCompare();

        //! Load a profile of patterns
        bool fromFile(string filename);
        //! Save to a file
        bool toFile(string filename);

        //! Predict the class of a sample
        int classify(std::vector<double> input, int lastClass);

        //! Compute the differences. We assume that the data is between 0 and 255 !
        void transform(vector<double> data);
       
    private:
        ublas::matrix<int> mPatterns;
        std::vector<int> mTargetClass;
        double mThreshold;

        vector<int> mEnergy;
        vector<double> mResult;        

        string mFilename;
};

#endif

