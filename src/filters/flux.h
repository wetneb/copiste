#ifndef INCLUDED_FLUXH
#define INCLUDED_FLUXH

#include "filters/filter.h"

//! A filter for computing the amount of variations of a given feature (absolute value of the discrete derivative)
class Flux : public Filter
{
    public:
        //! Compute the transformation : compute the "flux"
        void transform(vector<float> data);

        //! Get the number of available values (the same as in the original feature)
        int size();

        //! Return the asked value (indices are the same as in the original feature)
        float value(int index = 0);

    private:
        vector<float> mLastVect;
        vector<float> mResult;
};

#endif
