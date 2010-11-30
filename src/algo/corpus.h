#ifndef INCLUDEDCORPUSH
#define INCLUDEDCORPUSH

#include <string>

using namespace std;

//! Stores a set of training examples for a neural net
class Corpus;

#include "nnetwork.h"

class Corpus
{
    public:
        //! Loads the corpus from an XML file
        bool load(string file);

        //! Trains a given network to match the corpus
        void train(NNetwork &network);

        //! Writes to the standard output the set of elements
        void display();

        //! Returns the number of elements (i.e. vectors) contained in the corpus
        unsigned int size();

        //! Returns the number of inputs of each training example (i.e. the number of coordinates of each vector)
        unsigned int dimension();

        //! Returns the elem pointed by the given index
        float* elem(unsigned int index);

    private:
        float** mPool;
        int mSize;
        int mDimension;
};


#endif
