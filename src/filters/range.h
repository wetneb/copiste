#ifndef INCLUDED_RANGEH
#define INCLUDED_RANGEH

#include "filters/filter.h"

//! A class for computing the average between a set of features (usually for the spectrum)
class RangeFilter : public Filter
{
    public:
        //! Do the actual computation on the features
        void transform(vector<float> data);

        //! Returns the number of available features : 1
        int size() { return 1; }

        //! Get the result (only one available, i.e. index = 0)
        float value(int index = 0) { return mAverage; }

        //! Set a int parameter
        void setInt(string key, int value);

        //! Get a int parameter
        int getInt(string key);

    private:
        int mStart;
        int mEnd;
        float mAverage;
};

#endif
