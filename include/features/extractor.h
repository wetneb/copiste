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


#ifndef INCLUDED_EXTRACTORH
#define INCLUDED_EXTRACTORH

#include <vlc/vlc.h>
#include <iostream>

using namespace std;

/**
 * \class FeatureExtractor
 * \brief An abstract class, used as an interface between the processing unit and the features algorithms
 */
class FeatureExtractor
{
    public:
        //! Constructor. Can set the typical chunk size.
        FeatureExtractor(int chunkSize = 0) { ; }

        //! Virtual destructor : enables each extractor to delete arrays
        virtual ~FeatureExtractor() { ; }

        //! Run the algorithm and store the results
        virtual bool extract(uint16_t* data, int size) = 0;

        //! Retrive the results (from the index). The values are usually between -1 and 1
        virtual float value(int index = 0) = 0;

        //! Get the number of available values
        virtual int size() = 0;

        //! Set a float parameter
        virtual void setFloat(string key, float value) { ; }

        //! Set a int parameter
        virtual void setInt(string key, int value) { ; }

        //! Get a float parameter
        virtual float getFloat(string key) { return 0; }

        //! Get a int parameter
        virtual int getInt(string key) { return 0; }

        //! Clear the memory of the extractor (so that the previously computed results don't infer to the next ones)
        virtual void clear() { ; }
};


#endif
