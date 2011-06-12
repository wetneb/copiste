#ifndef INCLUDED_FILTERH
#define INCLUDED_FILTERH

#include "features/extractor.h"
#include <vector>

using namespace std;

//! Abstract class standing for a feature filter (counts the high / low values, computes the average…)
class Filter : public FeatureExtractor
{
    public:
        //! Default constructor : does nothing yet.
        Filter() : mExtr(0) { ; }

        //! Binds the filter to the output of an extractor (or another filter). Returns the old one.
        virtual FeatureExtractor* bind(FeatureExtractor* extr);

        /** \brief Extract data
         * This method should not be reimplemented by the user.
         * The given parameters are ignored : the data is drawn from the upstream extractor without
         * taking care of the raw data.
         */
        bool extract(uint16_t* data = 0, int size = 0);

        //! Do the actual computation on the features
        virtual void transform(vector<float> data) = 0;

    protected:
        FeatureExtractor* mExtr;
};

#endif
