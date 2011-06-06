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
        FeatureExtractor* bind(FeatureExtractor* extr)
        {
            FeatureExtractor* old = mExtr;
            mExtr = extr;
            return old;
        }

        //! Extract data
        bool extract(uint16_t* data = 0, int size = 0)
        {
            if(mExtr)
            {
                vector<float> data;
                data.resize(mExtr->size());
                for(unsigned int i = 0; i < data.size(); i++)
                    data[i] = mExtr->value(i);
                transform(data);
            }

            return true;
        }

        virtual void transform(vector<float> data) = 0;

    protected:
        FeatureExtractor* mExtr;
};

#endif
