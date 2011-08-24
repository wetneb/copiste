#ifndef INCLUDED_CENTROIDH
#define INCLUDED_CENTROIDH

#include "filters/filter.h"

//! A filter for computing the centroid of a feature (i.e. the average indice). See code.
class CentroidFilter : public Filter
{
    public:
        //! Compute the feature
        void transform(vector<float> data);

        //! Returns the result (available : "centroid")
        float value(int index = 0) { return mCentroid; }

        //! Size is 1
        int size() { return 1; }

    private:
        float mCentroid;
};

#endif
