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

#ifndef INCLUDEDNEURALNETWORKH
#define INCLUDEDNEURALNETWORKH

#include <string>

// Math functions
#include <cmath>
#include <cstdlib>

// Matrix and vector management
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>

// Serialization
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

// Temporaire
#include "corpus.h"


using namespace boost::numeric;

float sigmoid(float x);

//! A sightly better neural network
class NeuralNetwork
{
    public:
       //! Loads a neural network from a file if specified
       NeuralNetwork(std::string file = "");

       /** \brief Creates a fresh network with given input and hidden layer sizes)
         * The output layer is always a single neuron).
         * geometry[i] stores the size of the ith layer (not counting the bias unit)
         * The last integer is the size of the last hidden layer.
         */
       void reset(std::vector<int> geometry);

       //! Loads a network from a file. Returns true if it succeded.
       bool fromFile(std::string file);

       //! Save the current network to a file.
       bool toFile(std::string file);

       //! Train the network on a corpus
       float train(Corpus &c, float rate, float regularization);

       //! Get the output of the network on a given input
       float classify(std::vector<float> input);

       //! See how well the network does on a given corpus (between 0, not accurate, and 1, accurate)
       float accuracy(Corpus &c);

       //! Randomize the weights of the network
       void randomize();

       /// Parameters

       //! Number of layers of the network (hidden layers + output layer)
       unsigned int nbLayers() { return mLayers.size(); }

   private:
       std::vector< ublas::matrix<float> > mLayers;

       float gradientDescent(ublas::matrix<float> &ds, ublas::vector<float> &tv, float rate, unsigned int steps);

       //! Get the output of the network on a given set of inputs
       ublas::matrix<float> classify(ublas::matrix<float> input);

        //! Gradient of the cost function
       std::vector< ublas::matrix<float> > gradient(ublas::matrix<float> &ds, ublas::vector<float> &tv, float regularization = 0);

       //! Gradient checking
       std::vector< ublas::matrix<float> > gradientChecking(ublas::matrix<float> &ds, ublas::vector<float> &tv, float regularization = 0, float epsilon = 0.0001);

       //! Cost function computation
       float costFunction(ublas::matrix<float> &ds, ublas::vector<float> &tv, float regularization = 0);

       /// Corpus management

       //! Create a ublas::matrix from an array
       static ublas::matrix<float> createDataset(Corpus &c);

       //! Create a ublas::vector from an array
       static ublas::vector<float> createTargetVector(Corpus &c);

       /// Utility

       //! Add a 1 at the end of a vector
       static ublas::vector<float> addOne(ublas::vector<float> v);

       //! Add 1s at the bottom of a matrix
       static ublas::matrix<float> addOne(ublas::matrix<float> v);

       //! Remove 1s at the bottom of a matrix
       static ublas::matrix_range<ublas::matrix<float> > removeOnes(ublas::matrix<float> m);

       //! Get the j-th column of a matrix as a matrix
       static ublas::matrix_range<ublas::matrix<float> > jthCol(ublas::matrix<float> m, size_t j);

       //! Apply a function element-wise to a vector
       static ublas::vector<float> elementWise(ublas::vector<float> v, float (*f)(float));

       //! Apply a function element-wise to a matrix
       static ublas::matrix<float> elementWise(ublas::matrix<float> v, float (*f)(float));

       //! Create a matrix from a vector
       static ublas::matrix<float> vecToMat(ublas::vector<float> v);

       //! Create a vector from a matrix (unroll it)
       static ublas::vector<float> unroll(ublas::matrix<float> m);

        //! Create a vector from a matrix (unroll it)
       static ublas::matrix<float> roll(ublas::vector<float> m, unsigned int r, unsigned int c);

       //! Internal usage : for load and save
       friend class boost::serialization::access;
       template<class Archive>
       void serialize(Archive & ar, const unsigned int version);

};


#endif
