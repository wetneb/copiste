#include "filter.h"

FeatureExtractor* Filter::bind(FeatureExtractor* extr)
{
    FeatureExtractor* old = mExtr;
    mExtr = extr;
    return old;
}

//! Extract data
bool Filter::extract(uint16_t* dat, int size)
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
