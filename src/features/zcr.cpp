#include "features/zcr.h"

//! Run the algorithm and store the results
bool ZCRExtr::extract(uint16_t* data, int size)
{
    if(size)
    {
        int crossingsCount = 0;
        uint16_t zero = (-1);
        zero >>= 1;
        bool higher = data[0] > zero;

        for(int i = 1; i < size; i++)
        {
            if(higher != (data[i] > zero))
            {
                higher = !higher;
                crossingsCount++;
            }
        }

        mZCR = (float)crossingsCount / size;
    }
    return size;
}

