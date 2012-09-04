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

#ifndef INCLUDEDABSTRACTCLASSIFIERH
#define INCLUDEDABSTRACTCLASSIFIERH

#include <string> 
#include <vector>

#include "corpus.h"

//! A generic classifier interface (can be multi-class)
class AbstractClassifier
{
    public:
        //! Load the classifier from a file. Returns true in case of success.
        virtual bool fromFile(std::string file) = 0;
        //! Save the classifier to a file.
        virtual bool toFile(std::string file) = 0;

        //! Predict the class of an input vector. The last classification may not be used for the computation.
        virtual int classify(std::vector<double> input, int lastClass) = 0;

        //! Returns the number of input features
        virtual unsigned int dimension() = 0;
};

#endif

