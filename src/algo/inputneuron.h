#ifndef INCLUDEDINPUTNEURON
#define INCLUDEDINPUTNEURON

#include <string>

#include "algo/abstractneuron.h"

using namespace std;

//! A dummy neuron, returning an arbitrary value
class InputNeuron : public AbstractNeuron
{
    public:
        InputNeuron(string name="Input") : mName(name), mValue(1) { ; }

        neural_value value() { return mValue; }

        //! Sets the value of the neuron
        void setValue(neural_value value) { mValue = value; }

        //! An input neuron is orphan (this method should never be called)
        neural_value error(AbstractNeuron* parent) { return 0; }

        //! Randomizing the weights of an input neuron has no effect
        void randomize(bool spread = false) { ; }

        //! An input neuron doesn't have any weights
        void computeWeights(neural_value rate) { ; }

        //! Spreading error from an input neuron has no effect
        void spreadError() { ; }

        //! Training an input neuron has no effect
        void train(neural_value goal, float variation = 1) { ; }

        string str() const { return string("<") + mName + " : >"; }

        string name() const { return mName; }

        // An input neuron has no cache
        void clean() { ; }

        // An input neuron doesn't care for its children
        void addChild(AbstractNeuron* child) { ; }
        void forgetChild(AbstractNeuron* child) { ; }

    private:
        string mName;
        neural_value mValue;
};

#endif
