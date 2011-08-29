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

        bool write(QDomElement elem) { return false; }

    private:
        string mName;
        neural_value mValue;
};

#endif
