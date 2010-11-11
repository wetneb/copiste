#ifndef INCLUDEDABSTRACTPERCEPTRONH
#define INCLUDEDABSTRACTPERCEPTRONH

#include <iostream>

using namespace std;

//! An abstract perceptron
class AbstractPerceptron
{
    public:
        //! Default constructor. Does nothing
        //virtual AbstractPerceptron();

        //! Returns the output of the perceptron
        virtual float value() const = 0;

        //! Adapt the perceptron to match a given output
        virtual void train(float value, float variation = 1) = 0;

        //! Display the perceptron as a string
        virtual string str() const = 0;

        //! Returns the name of the perceptron
        virtual string name() const = 0;
};

#endif
