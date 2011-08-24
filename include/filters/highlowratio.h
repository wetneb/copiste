#ifndef INCLUDED_HIGHLOWRATIOH
#define INCLUDED_HIGHLOWRATIOH

#include "filters/filter.h"

#define DEFAULT_HIGH_LOW_WINDOW_SIZE 100

/**
 * \brief Counts the number of high or low values on a given feature
 * It computes the average and compares each value to the average multiplied by "bound",
 * the float can be set in the XML file.
 * The user can also decide if the number of values higher or lower should be counted.
 */
class HighLowRatioFilter : public Filter
{
    public:
        //! Constructor : does nothing by default
        HighLowRatioFilter() : mBound(1),
                               mHigh(true),
                               mWindowSize(DEFAULT_HIGH_LOW_WINDOW_SIZE),
                               mCurrentIndex(0)
                               { ; }

        //! Cleans the memory
        ~HighLowRatioFilter();

        //! Extracts the feature by counting the high/low values
        void transform(vector<float> data);

        //! Retrive the results
        float value(int index = 0) { return mResults[index] / mWindowSize; }

        //! Set a float parameter (available : bound)
        void setFloat(string key, float value);

        //! Set a int parameter (available : high, windowSize)
        void setInt(string key, int value);

        //! Get a float parameter (available : bound)
        float getFloat(string key);

        //! Get a int parameter (available : high, windowSize)
        virtual int getInt(string key);

        //! Returns the size of the results (which is the size of the upstream feature)
        int size() { return mResults.size(); }

    private:
        void reallocate(unsigned int newSize);

        float mBound;
        bool mHigh;
        int mWindowSize;
        vector<float> mResults;
        vector<float> mAverage;
        vector<float*> mMemory;
        unsigned int mCurrentIndex;
};

#endif
