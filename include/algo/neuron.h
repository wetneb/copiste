/*
 * This file is part of Copiste.
 *
 * Copiste is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 *  Copiste is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Copiste.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef INCLUDEDNEURONH
#define INCLUDEDNEURONH

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <QDomNode>

#include "algo/abstractneuron.h"
#include "algo/nnetwork.h"

#define EPSILON 0.1

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
        void load(QDomElement element, const NNetwork* network);
        //! Writes down the network to a XML file
        bool write(QDomElement elem);

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
