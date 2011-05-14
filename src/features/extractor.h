#ifndef INCLUDED_EXTRACTORH
#define INCLUDED_EXTRACTORH

#include <vlc/vlc.h>
#include <iostream>

using namespace std;

//! TODO : ideally, this class could implement parameter handling (hash table, or whatever)

/**
 * \class FeatureExtractor
 * \brief An abstract class, used as an interface between the processing unit and the features algorithms
 */
class FeatureExtractor
{
    public:
        //! Constructor. Can set the typical chunk size.
        FeatureExtractor(int chunkSize = 0) { ; }

        //! Run the algorithm and store the results
        virtual bool extract(uint16_t* data, int size) = 0;

        //! Retrive the results (from the index). The values are usually between -1 and 1
        virtual float value(int index = 0) = 0;

        //! Get the number of available values
        virtual int size() = 0;

        //! Set a float parameter
        virtual void setFloat(string key, float value) = 0;

        //! Set a int parameter
        virtual void setInt(string key, int value) = 0;

        //! Get a float parameter
        virtual float getFloat(string key) = 0;

        //! Get a int parameter
        virtual int getInt(string key) = 0;
};


#endif
