#ifndef INCLUDEDPERCEPTRONH
#define INCLUDEDPERCEPTRONH

#include <string>
#include <sstream>
#include <vector>
#include <QDomNode>

#include "algo/abstractperceptron.h"
#include "algo/nnetwork.h"

using namespace std;

float sigmoid(float x);

//! Implements a generic perceptron, whose value is computed from other perceptron's values
class Perceptron : public AbstractPerceptron
{
    public:
        //! Default constructor. Only gives a name to the perceptron.
        Perceptron(string name="Perceptron") : mName(name) { ; }

        //! Sets up the perceptron with an XML node and a network
        void load(QDomNode node, const NNetwork* network);

        //! Adds a parent, i.e. another perceptron involved in computing the value of this perceptron
        void addParent(AbstractPerceptron* parent, float weight);
        //! Removes a parent. Does nothing if the given perceptron wasn't a parent before.
        void removeParent(AbstractPerceptron* parent);
        //! Changes the weight of a parent.
        void setParentWeight(AbstractPerceptron* parent, float weight);
        //! Returns the number of parents this perceptron has.
        int nbParents() { return mParents.size(); }

        //! Computes the output value of this perceptron.
        float value() const;

        //! Trains the perceptron
        void train(float value, float variation = 1) { ; }

        //! Displays the perceptron as a string.
        string str() const;

        //! Returns the name of the perceptron
        string name() const;
    private:
        string mName;
        vector<AbstractPerceptron*> mParents;
        vector<float> mWeights;
};

#endif
