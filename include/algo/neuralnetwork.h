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

double sigmoid(double x);

//! A slightly better neural network
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
       double train(Corpus &c, double rate, double regularization, int nbIter, bool debug = false);

       //! Get the output of the network on a given input
       double classify(std::vector<double> input);

       //! See how well the network does on a given corpus (between 0, not accurate, and 1, accurate)
       double accuracy(Corpus &c);

       //! Randomize the weights of the network
       void randomize();

       /// Parameters

       //! Number of layers of the network (hidden layers + output layer)
       unsigned int nbLayers() { return mLayers.size(); }

       //! Input dimension of the network (size of the input layer)
       unsigned int dimension();

   private:
       std::vector< ublas::matrix<double> > mLayers;

       double gradientDescent(ublas::matrix<double> &ds, ublas::vector<double> &tv, double rate, unsigned int steps);

       //! Get the output of the network on a given set of inputs
       ublas::matrix<double> classify(ublas::matrix<double> input);

        //! Gradient of the cost function
       std::vector< ublas::matrix<double> > gradient(ublas::matrix<double> &ds, ublas::vector<double> &tv, double regularization = 0);

       //! Gradient checking
       std::vector< ublas::matrix<double> > gradientChecking(ublas::matrix<double> &ds, ublas::vector<double> &tv, double regularization = 0, double epsilon = 0.0001);

       //! Cost function computation
       double costFunction(ublas::matrix<double> &ds, ublas::vector<double> &tv, double regularization = 0);

       /// Corpus management

       //! Create a ublas::matrix from an array
       static ublas::matrix<double> createDataset(Corpus &c);

       //! Create a ublas::vector from an array
       static ublas::vector<double> createTargetVector(Corpus &c);

       /// Utility

       //! Add a 1 at the end of a vector
       static ublas::vector<double> addOne(ublas::vector<double> v);

       //! Add 1s at the bottom of a matrix
       static ublas::matrix<double> addOne(ublas::matrix<double> v);

       //! Remove 1s at the bottom of a matrix
       static ublas::matrix_range<ublas::matrix<double> > removeOnes(ublas::matrix<double> m);

       //! Get the j-th column of a matrix as a matrix
       static ublas::matrix_range<ublas::matrix<double> > jthCol(ublas::matrix<double> m, size_t j);

       //! Apply a function element-wise to a vector
       static ublas::vector<double> elementWise(ublas::vector<double> v, double (*f)(double));

       //! Apply a function element-wise to a matrix
       static ublas::matrix<double> elementWise(ublas::matrix<double> v, double (*f)(double));

       //! Create a matrix from a vector
       static ublas::matrix<double> vecToMat(ublas::vector<double> v);

       //! Create a vector from a matrix (unroll it)
       static ublas::vector<double> unroll(ublas::matrix<double> m);

        //! Create a vector from a matrix (unroll it)
       static ublas::matrix<double> roll(ublas::vector<double> m, unsigned int r, unsigned int c);

       //! Internal usage : for load and save
       friend class boost::serialization::access;
       template<class Archive>
       void serialize(Archive & ar, const unsigned int version);

};


#endif