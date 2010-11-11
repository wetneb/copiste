#ifndef INCLUDEDNNETWORKH
#define INCLUDEDNNETWORKH

#include <vector>
#include <QHash>

class NNetwork;

#include "algo/abstractperceptron.h"
#include "algo/perceptron.h"
#include "algo/inputperceptron.h"

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
        unsigned int addInput(InputPerceptron* input);
        //! Removes an input
        void removeInput(unsigned int id);

        //! Returns a pointer to the perceptron named so.
        AbstractPerceptron* getPerceptronByName(string name) const;

        //! Same, using QString instead of std::string
        AbstractPerceptron* getPerceptronByName(QString name) const;

        //! Computes the output from an input vector
        float compute(vector<int> input);

        //! Computes the output from an input vector and train to match a given goal
        float train(vector<int> input, float goal, float variation = 1);

        //! Displays the network
        void display();

    private:
        vector<InputPerceptron*> mInputs;
        QHash<QString, AbstractPerceptron*> mPerceptrons;
        AbstractPerceptron* mLastPerceptron;
};

#endif
