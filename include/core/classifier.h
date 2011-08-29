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


#ifndef INCLUDED_CLASSIFIER
#define INCLUDED_CLASSIFIER

#include "core/soundanalyser.h"
#include "algo/nnetwork.h"

//! A class reading a file and classifying its content
class Classifier : public SoundAnalyser
{
    public:
        //! Constructor
        Classifier(bool verbose = true);

        //! Set the network that should be used for classification
        void setNetwork(NNetwork *net);

        //! Get the class of the content
        string getClass();

    private:
        NNetwork *mNet;
        bool mVerbose;
};

#endif
