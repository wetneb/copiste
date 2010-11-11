#ifndef INCLUDEDINPUTPERCEPTRON
#define INCLUDEDINPUTPERCEPTRON

#include <string>

#include "algo/abstractperceptron.h"

using namespace std;

//! A dummy perceptron, returning an arbitrary value
class InputPerceptron : public AbstractPerceptron
{
    public:
        InputPerceptron(string name="Input") : mName(name), mValue(1) { ; }

        float value() const { return mValue; }

        //! Sets the value of the perceptron
        void setValue(float value) { mValue = value; }

        void train(float value, float variation = 1) { ; }

        string str() const { return string("<") + mName + " : >"; }

        string name() const { return mName; }

    private:
        string mName;
        float mValue;
};

#endif
