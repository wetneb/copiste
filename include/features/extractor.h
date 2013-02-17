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
#include <deque>

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
        virtual bool extract(int16_t* data, int size, int channels) = 0;
        //! Idem, but the buffer starts at index start instead (circular buffer)
        bool extract(int16_t* data, int size, int channels, size_t start); 

        //! Retrieve the results (from the index). Some values may be out of bounds.
        virtual float value(int index = 0) = 0;
        //! Retrieve the results, with the bounds respected (we apply a min / max)
        virtual float safeValue(int index = 0)
        {  return std::min(max(), std::max(min(), value(index))); }
        //! Retrieve the results, with other bounds
        virtual float value(int index, float low, float high)
        { return low + (high-low)*(safeValue(index) - min())/(max() - min()); }

        //! Get the number of available values
        virtual int size() = 0;
        //! Get the minimum value outputted (we assume that each value from a single extractor is about as big as the others)
        virtual float min() { return -1; }
        //! Get the maximum value outputted
        virtual float max() { return 1; }

        //! Set a float parameter
        virtual void setFloat(string key, float value) { ; }
        //! Set a int parameter
        virtual void setInt(string key, int value) { ; }
        //! Get a float parameter
        virtual float getFloat(string key) { return 0; }
        //! Get a int parameter
        virtual int getInt(string key) { return 0; }
        //! Set a string parameter
        virtual void setString(string key, string value) { ; }
        //! Get a string parameter
        virtual string getString(string key) { return ""; }

        //! Clear the memory of the extractor (so that the previously computed results don't infer to the next ones)
        virtual void clear() { ; }
};


#endif
