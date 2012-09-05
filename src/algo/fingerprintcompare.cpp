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

#include "algo/fingerprintcompare.h"

FingerprintCompare::FingerprintCompare() 
{
    ;
}

bool FingerprintCompare::fromFile(std::string filename)
{
    Corpus c(filename);
    mPatterns = c.asFingerprints();
    // \todo
    //
    return true;
}

bool FingerprintCompare::toFile(std::string filename)
{
    // \todo
    return true;
}

void FingerprintCompare::addFingerprint(ublas::vector<int> vec, int targetClass)
{
    mPatterns.resize(mPatterns.size1()+1, mPatterns.size2());
    ublas::matrix_column<ublas::matrix<int> > mc(mPatterns, mPatterns.size1()-1);
    mc = vec;
    mTargetClass.push_back(targetClass);
    mResult.resize(mPatterns.size1(), 0);
    mEnergy[mPatterns.size1()-1] = computeEnergy(vec);
}

int FingerprintCompare::classify(std::vector<double> input, int lastClass)
{
    transform(input);
    
    if(mResult.size() == 0)
        return lastClass;

    // Get the index of the minimum in the distances
    int imin = 0;
    for(unsigned int i = 1; i < mResult.size(); i++)
        if(mResult[i] < mResult[imin])
            imin = i;

    if(mResult[imin] < mThreshold)
        return imin;
    else
        return lastClass;
}

void FingerprintCompare::transform(std::vector<double> data)
{
    if(data.size() != mPatterns.size2())
        std::cerr << "Warning, profile and stream sizes don't match : "
         << mPatterns.size2() << " vs " << data.size() << " (FingerprintCompare)" << std::endl;
    
    mResult.resize(mPatterns.size1());
    
    double tmp = 0;
    for(unsigned int i = 0; i < mPatterns.size1(); i++)
    {
        double sum = 0;
        for(unsigned int j = 0; j < mPatterns.size2(); j++)
        {
            tmp = mPatterns(i,j) - data[j];
            sum += tmp * tmp;
        }
        mResult[i] = sum / mEnergy[i];
    }
}

int FingerprintCompare::computeEnergy(ublas::vector<int> vec)
{
    int sum = 0;
    for(unsigned int i = 0; i < vec.size(); i++)
        sum += vec[i] * vec[i];
    return sum;
}

