#ifndef INCLUDEDNEURONH
#define INCLUDEDNEURONH

#include <string>
#include <sstream>
#include <vector>
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

        //! Sets up the neuron with an XML node and a network
        void load(QDomNode node, const NNetwork* network);

        //! Adds a parent, i.e. another neuron involved in computing the value of this neuron
        void addParent(AbstractNeuron* parent, float weight);
        //! Removes a parent. Does nothing if the given neuron wasn't a parent before.
        void removeParent(AbstractNeuron* parent);
        //! Changes the weight of a parent.
        void setParentWeight(AbstractNeuron* parent, float weight);
        //! Returns the number of parents this neuron has.
        int nbParents() { return mParents.size(); }

        //! Computes the output value of this neuron.
        float value();

        //! Compute and return the error of this neuron
        float error();

        //! Trains the neuron
        void train(float goal, float rate = 1);

        //! Cleans the cache of the neuron
        void clean();

        //! Displays the neuron as a string.
        string str() const;

        //! Returns the name of the neuron
        string name() const;
    private:
        string mName;
        vector<AbstractNeuron*> mParents;
        vector<float> mWeights;

        float mValue;
        float mError;
        bool mValueCached;
        bool mErrorCached;
};

#endif
