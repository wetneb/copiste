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

#include "algo/compare.h"

FingerprintCompare::FingerprintCompare() 
{
    ;
}

bool FingerprintCompare::fromFile(string filename)
{
    QDomDocument doc;

    QFile file(filename.c_str());
    if(!file.open(QIODevice::ReadOnly))
    {
        std::cerr << "Unable to open the file " << filename << std::endl;
        return false; 
    }
    
    QDomElement rootElem = doc.documentElement();

    if(rootElem.tagName() != "profile")
    {
        std::cerr << filename << " : Invalid markup : expected <profile>." << std::endl;
        return false; 
    }

    int nFingerprints = rootElem.childNodes().length();
    int nFeatures = rootElem.attribute("features", "0").toInt();
    mPatterns.resize(nFingerprints, nFeatures);
    mTargetClass.resize(nFingerprints);

    QDomNode currNode = rootElem.firstChild();
  
    int nValidFps = 0;
    while(!currNode.isNull())
    {
        QDomElement elem = currNode.toElement();
        if(elem.tagName() != "fingerprint")
        {
            std::cerr << filename << " : Ignored markup <"
                << elem.tagName().toStdString() << ">." << std::endl;
        }
        else
        {
            mTargetClass[nValidFps] = elem.attribute("class", "0").toInt();
            std::istringstream stream(elem.text().toStdString());
            try
            {
                ublas::vector<double> fp;
                boost::archive::text_iarchive ar(stream);
                ar & fp;
                ublas::matrix_column<ublas::matrix<int> > mc(mPatterns, nValidFps);
                mc = fp;
            }
            catch(boost::archive::archive_exception ex)
            {
                std::cerr << filename << " : Error, invalid fingerprint : " << ex.what() << std::endl;
                return false;
            }

            nValidFps++;
        }
    }

    mPatterns.resize(nValidFps, nFeatures);

    return true;
}

bool FingerprintCompare::toFile(string filename)
{
    return true;
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

void FingerprintCompare::transform(vector<double> data)
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


