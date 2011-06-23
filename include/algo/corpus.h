#ifndef INCLUDEDCORPUSH
#define INCLUDEDCORPUSH

#include <string>
#include <cmath>
#include <QFile>

using namespace std;

class Corpus;

#include "abstractneuron.h"
#include "nnetwork.h"

//! Stores a set of training examples for a neural net
class Corpus
{
    public:
        /// Construction, destruction

        //! Default constructor. Loads a file if specified.
        Corpus(string file = "");
        //! Constructor, creating an empty corpus of dimension dim
        Corpus(int dim);
        //! Frees the memory
        ~Corpus();
        //! Erases the corpus and start a new one with the given dimension
        void erase(int dimension = 0);

        /// Input, output

        //! Loads the corpus from an XML file
        bool load(string file, bool verbose = false);
        //! Write the corpus to a file
        void write(string file);
        //! Writes to the standard output the set of elements
        void display() const;

        /// Computing

        /**
         * \brief Trains a given network to match the corpus (writing error rates in **history)
         * The history field must be a pointer to a pointer, which has to be deleted then by the user.
         */
        int train(NNetwork &network, float learningRate, int maxPasses, float **history = 0, bool random = true);
        //! Displays the corpus accuracy
        float accuracy(NNetwork &network, bool verbose = false) const;

        /// Accessors

        //! Returns the number of elements (i.e. vectors) contained in the corpus
        unsigned int size() const;
        //! Returns the number of inputs of each training example (i.e. the number of coordinates of each vector)
        unsigned int dimension() const;

        //! Returns the elem pointed by the given index
        neural_value* elem(unsigned int index) const { return mPool[index]; }
        //! Add a sample to the corpus
        void addElem(neural_value* elem);
        //! Retuns the bounds of the corpus (the vector is : min_1 max_1 min_2 max_2 ... min_n max_n)
        vector<float> bounds() const;

    private:
        neural_value** mPool;
        int mSize; // Stores the number of elements stored in the pool
        int mDimension;

        int mPoolSize; // Stores the actual capacity of the pool
};

#endif
