#ifndef INCLUDEDNNETWORKH
#define INCLUDEDNNETWORKH

#include <vector>
#include <sstream>
#include <QHash>
#include <cstdlib>

class NNetwork;

#include "algo/abstractneuron.h"
#include "algo/neuron.h"
#include "algo/inputneuron.h"

using namespace std;

/** \todo Add a _real_ layer support : curently, the software can produce a XML file
* which it won't be able to read back later (the order of the neurons has to be managed.
*/

//! A neural network.
class NNetwork
{
    public:
        //! Default constructor.
        NNetwork(string fileName = "");
        //! Destructor : calls clear()
        ~NNetwork();
        //! Clears the network (deletes everything)
        void clear();

        //! Loads a network from an XML file
        bool load(string fileName);
        //! Writes the netowrk to a file
        void write(string filename);


        //! Generates a simple network for a given dimension (number of inputs) and depth (number of layers, excluding the input and the output layer)
        void generate(unsigned int dimension, unsigned int depth);
        //! Randomizes the weights, between -1 and 1 (useful before training)
        void randomize();

        //! Adds an input. Returns the ID of the input
        unsigned int addInput(InputNeuron* input);
        //! Removes an input
        void removeInput(unsigned int id);

        //! Returns a pointer to the neuron named so.
        AbstractNeuron* getNeuronByName(string name) const;

        //! Same, using QString instead of std::string
        AbstractNeuron* getNeuronByName(QString name) const;

        //! Computes the output from an input vector
        float compute(vector<neural_value> input);

        //! Computes the output from an input vector and train to match a given goal
        float train(vector<neural_value> input, neural_value goal, float rate = 1);

        //! Cleans (empties the cache of the network)
        void clean();

        //! Displays the network
        void display();

    private:
        vector<InputNeuron*> mInputs;
        QHash<QString, AbstractNeuron*> mNeurons;
        AbstractNeuron* mLastNeuron;
};

#endif
