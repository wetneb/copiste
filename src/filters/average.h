#ifndef INCLUDED_AVERAGEH
#define INCLUDED_AVERAGEH

#include "filters/filter.h"

//! A filter for computing the average of a value
class AverageFilter : public Filter
{
    public:
        //! Default constructor : does nothing
        AverageFilter() : Filter(), mHistory(0), mAverage(0), mSize(0), mDimension(0), mCurrentIndex(0) { ; }

        //! Set the input
        FeatureExtractor* bind(FeatureExtractor* extr);

        //! Run the algorithm and store the results
        void transform(vector<float> data);

        //! Retrive the results (from the index). The values are usually between -1 and 1
        float value(int index = 0);

        //! Get the number of available values
        int size();

        //! Set a float parameter
        void setFloat(string key, float value);

        //! Set a int parameter (available : size)
        void setInt(string key, int value);

        //! Get a float parameter
        float getFloat(string key);

        //! Get a int parameter  (available : size)
        int getInt(string key);

    private:
        float** mHistory;
        float* mAverage;
        int mSize;
        int mDimension;
        int mCurrentIndex;
};

#endif
