#ifndef INCLUDEDCORPUSH
#define INCLUDEDCORPUSH

#include <string>
#include <QFile>

using namespace std;

//! Stores a set of training examples for a neural net
class Corpus;

#include "abstractneuron.h"
#include "nnetwork.h"

class Corpus
{
    public:
        //! Default constructor. Loads a file if specified.
        Corpus(string file = "");

        //! Frees the memory
        ~Corpus();

        //! Loads the corpus from an XML file
        bool load(string file);

        //! Trains a given network to match the corpus
        int train(NNetwork &network, float learningRate, int maxPasses);

        //! Displays the corpus compliance
        void compliance(NNetwork &network);

        //! Writes to the standard output the set of elements
        void display();

        //! Returns the number of elements (i.e. vectors) contained in the corpus
        unsigned int size();

        //! Returns the number of inputs of each training example (i.e. the number of coordinates of each vector)
        unsigned int dimension();

        //! Returns the elem pointed by the given index
        neural_value* elem(unsigned int index);

        //! Erases the corpus
        void erase();

    private:
        neural_value** mPool;
        int mSize;
        int mDimension;
};


#endif
