#ifndef INCLUDEDCORPUSH
#define INCLUDEDCORPUSH

#include <string>
#include <cmath>
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
        bool load(string file, bool verbose = false);

        //! Write the corpus to a file
        void write(string file);

        //! Trains a given network to match the corpus (writing error rates in **history)
        int train(NNetwork &network, float learningRate, int maxPasses, float **history = 0, bool random = true);

        //! Displays the corpus accuracy
        float accuracy(NNetwork &network, bool verbose = false);

        //! Writes to the standard output the set of elements
        void display();

        //! Returns the number of elements (i.e. vectors) contained in the corpus
        unsigned int size();

        //! Returns the number of inputs of each training example (i.e. the number of coordinates of each vector)
        unsigned int dimension();

        //! Returns the elem pointed by the given index
        neural_value* elem(unsigned int index) { return mPool[index]; }

        //! Add a sample to the corpus
        void addElem(neural_value* elem);

        //! Retuns the bounds of the corpus
        vector<float> bounds();

        //! Erases the corpus
        void erase();

    private:
        neural_value** mPool;
        int mSize;
        int mDimension;

        int mPoolSize;
};

#endif
