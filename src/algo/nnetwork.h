#ifndef INCLUDEDNNETWORKH
#define INCLUDEDNNETWORKH

#include <vector>
#include <QHash>

class NNetwork;

#include "algo/abstractneuron.h"
#include "algo/neuron.h"
#include "algo/inputneuron.h"

using namespace std;

//! A neural network.
class NNetwork
{
    public:
        //! Default constructor. Does nothing.
        NNetwork() { ; }

        //! Loads a network from an XML file
        bool load(string fileName);

        //! Adds an input. Returns the ID of the input
        unsigned int addInput(InputNeuron* input);
        //! Removes an input
        void removeInput(unsigned int id);

        //! Returns a pointer to the neuron named so.
        AbstractNeuron* getNeuronByName(string name) const;

        //! Same, using QString instead of std::string
        AbstractNeuron* getNeuronByName(QString name) const;

        //! Computes the output from an input vector
        float compute(vector<int> input);

        //! Computes the output from an input vector and train to match a given goal
        float train(vector<int> input, neural_value goal, float rate = 1);

        //! Cleans (empties the cache) of the network
        void clean();

        //! Displays the network
        void display();

    private:
        vector<InputNeuron*> mInputs;
        QHash<QString, AbstractNeuron*> mNeurons;
        AbstractNeuron* mLastNeuron;
};

#endif
