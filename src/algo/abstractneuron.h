#ifndef INCLUDEDABSTRACTNEURONH
#define INCLUDEDABSTRACTNEURONH

#include <iostream>

using namespace std;

//! A typedef allowing us to change easely the hardware precision used in computations
typedef neural_value float;

//! An abstract neuron
class AbstractNeuron
{
    public:
        //! Default constructor. Does nothing
        //virtual AbstractNeuron();

        //! Returns the output value of the neuron
        virtual neural_value value() = 0;

        //! Adapt the neuron to match a given output
        virtual void train(neural_value value, float rate = 1) = 0;

        //! Return the weighted error for a given parent (weight is set to 1 if parent is 0)
        virtual float error(AbstractNeuron* parent = 0) = 0;

        //! Change the weights using the error and the cached output value of the neuron
        virtual void computeWeights(float rate) = 0;

        //! Tells to the neuron it has a new child
        virtual void addChild(AbstractNeuron* child) = 0;

        //! Tells to the neuron it has no more this neuron as a child
        //! (should be called in the destructor of each object which registered itself as a child).
        virtual void forgetChild(AbstractNeuron* child) = 0;

        //! Display the neuron as a string
        virtual string str() const = 0;

        //! Returns the name of the neuron
        virtual string name() const = 0;

        //! Cleans the cache (forgets the output value and the error, but keeps weights and parents)
        virtual void clean() = 0;
};

#endif
