#include "filters/centroid.h"

void CentroidFilter::transform(vector<float> data)
{
    float weightsSum = 0;
    float mean = 0;

    for(unsigned int i = 0; i < data.size(); i++)
    {
        weightsSum += data[i];
        mean += data[i]*(i+1);
    }

    mCentroid = mean / weightsSum;
}
