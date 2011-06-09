#include "features/ste.h"

//! Run the algorithm and store the results
bool STEExtr::extract(uint16_t* data, int size)
{
    if(size)
    {
        mSTE = 0;
        uint16_t zero = (-1);
        zero >>= 1;

        if(mSquare)
        {
            for(int i = 0; i < size; i++)
                mSTE += ((int)data[i] - zero) * ((int)data[i] - zero);
        }
        else
        {
            for(int i = 0; i < size; i++)
                mSTE += fabs(data[i]);
        }
    }
    return size;
}

//! Set a int parameter (available : "square", which is 0 or 1)
void STEExtr::setInt(string key, int value)
{
    if(key == "square")
        mSquare = value;
}

//! Get a int parameter (available : "square", which is 0 or 1)
int STEExtr::getInt(string key)
{
    int ret = 0;
    if(key == "square")
        ret = (mSquare ? 1 : 0);
    return ret;
}
