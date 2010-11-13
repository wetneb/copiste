#ifndef INCLUDEDABSTRACTNEURONH
#define INCLUDEDABSTRACTNEURONH

#include <iostream>

using namespace std;

//! An abstract neuron
class AbstractNeuron
{
    public:
        //! Default constructor. Does nothing
        //virtual AbstractNeuron();

        //! Returns the output value of the neuron
        virtual float value() = 0;

        //! Adapt the neuron to match a given output
        virtual void train(float value, float rate = 1) = 0;

        //! Return the weighted error for a given parent
        virtual float error(AbstractNeuron* parent) = 0;

        //! Display the neuron as a string
        virtual string str() const = 0;

        //! Returns the name of the neuron
        virtual string name() const = 0;

        //! Cleans the cache (forgets the output value and the error, but keeps weights and parents)
        virtual void clean() = 0;

        //! Returns the error of this neuron
        virtual float error() = 0;
};

#endif
