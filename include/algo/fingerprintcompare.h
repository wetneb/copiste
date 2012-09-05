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
#include <boost/numeric/ublas/vector.hpp>

#include <fstream>
#include <sstream>
#include <QtXml>

using namespace boost::numeric;

const double DEFAULT_PATTERN_COMPARE_THRESHOLD = 0.05;

//! Compares the values of a feature to a set of known patterns
class FingerprintCompare : public AbstractClassifier
{
    public:
        FingerprintCompare();

        //! Load a profile of patterns
        bool fromFile(std::string filename);
        //! Save to a file
        bool toFile(std::string filename);

        //! Predict the class of a sample
        int classify(std::vector<double> input, int lastClass);
        //! Add a new fingerprint
        void addFingerprint(ublas::vector<int> vec, int targetClass);

        //! Compute the differences. We assume that the data is between 0 and 255 !
        void transform(std::vector<double> data);

        //! Number of input features
        unsigned int dimension() { return mPatterns.size2(); }

    private:
        //! Energy is \sum_{f feature} f^2
        static int computeEnergy(ublas::vector<int> vec);

        ublas::matrix<int> mPatterns;
        std::vector<int> mTargetClass;
        double mThreshold;

        std::vector<int> mEnergy;
        std::vector<double> mResult;        
};

#endif

