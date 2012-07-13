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


#include "core/fingerprinter.h"

//! Constructor
Fingerprinter::Fingerprinter(bool verbose) : SoundAnalyser(), mVerbose(verbose)
{
    setNormalization(0, 255);
}

//! Create the fingerprint
ublas::vector<int> Fingerprinter::getFingerprint()
{
    waitComputed();

    // Create the fingerprint vector
    ublas::vector<int> fp(realDimension());
    int wrote = 0;
    for(unsigned int i = 0; i < nbFeatures(); i++)
    {
        if(isUsed(i))
        {
            for(unsigned int j = 0; j < nbElems(i); j++)
            {
                // Compute the average
                fp(wrote) = features(nbSamples()-1)[i][j];
                wrote++;

                if(mVerbose)
                    cout << fp[fp.size() - 1] << "\t";
            }
        }
    }
    if(mVerbose)
        cout << endl;

    // Send the input vector to the network

    return fp;
}

