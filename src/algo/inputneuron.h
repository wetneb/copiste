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

        float value() { return mValue; }

        //! Sets the value of the neuron
        void setValue(float value) { mValue = value; }

        // An input neuron is orphan (this method should never be called)
        float error(AbstractNeuron* parent) { return 0; }

        // Training an input neuron has no effect
        void train(float goal, float variation = 1) { ; }

        string str() const { return string("<") + mName + " : >"; }

        string name() const { return mName; }

        // An input neuron has no cache
        void clean() { ; }

        // An input neuron says always the truth (since it says what he has been said to say)
        float error() { return 0; }

    private:
        string mName;
        float mValue;
};

#endif
