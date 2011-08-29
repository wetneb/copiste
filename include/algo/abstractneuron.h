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


#ifndef INCLUDEDABSTRACTNEURONH
#define INCLUDEDABSTRACTNEURONH

#include <QDomNode>

#include <iostream>

using namespace std;

//! A typedef allowing us to change easely the hardware precision used in computations
typedef float neural_value;

//! An abstract neuron
class AbstractNeuron
{
    public:
        //! Default constructor. Does nothing
        //virtual AbstractNeuron();

        //! Returns the output value of the neuron
        virtual neural_value value() = 0;

        //! Randomizes the weights of the neuron. And do it for the parents if "spread" is set to true.
        virtual void randomize(bool spread = false) = 0;

        //! Adapt the neuron to match a given output
        virtual void train(neural_value value, float rate = 1) = 0;

        //! Return the weighted error for a given parent (weight is set to 1 if parent is 0)
        virtual float error(AbstractNeuron* parent = 0) = 0;

        //! Calculates the error of the parents
        virtual void spreadError() = 0;

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

        //! Writes the neuron to a DOM element
        virtual bool write(QDomElement elem) = 0;
};

#endif
