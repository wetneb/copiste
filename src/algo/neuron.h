#ifndef INCLUDEDNEURONH
#define INCLUDEDNEURONH

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <QDomNode>

#include "algo/abstractneuron.h"
#include "algo/nnetwork.h"

using namespace std;

float sigmoid(float x);

//! Implements a generic neuron, whose value is computed from other neuron's values
class Neuron : public AbstractNeuron
{
    public:
        //! Default constructor. Only gives a name to the neuron.
        Neuron(string name="Neuron");

        //! Default destructor. Tells its parents it exists no more.
        ~Neuron();

        //! Sets up the neuron with an XML node and a network
        void load(QDomNode node, const NNetwork* network);

        //! Adds a parent, i.e. another neuron involved in computing the value of this neuron
        void addParent(AbstractNeuron* parent, neural_value weight);
        //! Changes the weight of a parent.
        void setParentWeight(AbstractNeuron* parent, neural_value weight);
        //! Returns the weight of a parent.
        neural_value parentWeight(AbstractNeuron* parent);

        //! Registers a child.
        void addChild(AbstractNeuron *child);
        //! Forgets a child.
        void forgetChild(AbstractNeuron *child);

        //! Computes the output value of this neuron.
        neural_value value();
        //! Compute and return the error of this neuron. If a parent is given, the error is weighted by the parent's weight
        neural_value error(AbstractNeuron* parent = 0);
        //! Adapt weights using the error
        void computeWeights(float rate);
        //! Compute the error of the parents
        void spreadError();
        //! Randomizes the weights
        void randomize(bool spread = false);

        //! Trains the neuron
        void train(neural_value goal, float rate = 1);

        //! Cleans the cache of the neuron
        void clean();

        //! Displays the neuron as a string.
        string str() const;

        //! Returns the name of the neuron
        string name() const;
    private:
        string mName;
        vector<AbstractNeuron*> mParents;
        vector<AbstractNeuron*> mChildren;
        vector<neural_value> mWeights;

        neural_value mValue;
        neural_value mError;
        bool mValueCached;
        bool mErrorCached;
};

#endif
