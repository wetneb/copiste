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

 #include "algo/neuralnetwork.h"

 float sigmoid(float x) // This is actually the hyperbolic tangent
{
    return (2.0 / (1.0 + exp(-x))) - 1.0;
}

float d_sigmoid(float x)
{
    return 2.0*exp(-x)/((1.0 + exp(-x))*(1.0 + exp(-x)));
}



//! Loads a neural network from a file if specified
NeuralNetwork::NeuralNetwork(std::string file)
{

    load(file);
}

//! Creates a fresh network with given input and hidden layer sizes (the output layer is always a single neuron)
void NeuralNetwork::reset(std::vector<int> geometry)
{
    mLayers.resize(geometry.size());

    for(unsigned int i = 1; i < geometry.size(); i++)
        mLayers[i-1].resize(geometry[i], geometry[i-1] + 1, false);
    mLayers[nbLayers() - 1].resize(1, geometry[nbLayers() - 1] + 1);
}

//! Loads a network from a file. Returns true if it succeded.
bool NeuralNetwork::load(std::string file)
{
    return true;
}

//! Save the current network to a file.
bool NeuralNetwork::save(std::string file)
{
    return true;
}

//! Train the network on a corpus
float NeuralNetwork::train(Corpus c, float regularization)
{
    return 0;
}

//! Randomize the weights of the network
void NeuralNetwork::randomize()
{
    ;
}

//! Get the output of the network on a given input
float NeuralNetwork::classify(std::vector<float> input)
{
    ublas::vector<float> activation(input.size() + 1);
    for(unsigned int i = 0; i < input.size(); i++)
        activation[i] = input[i];
    activation[input.size()] = 1;

    for(unsigned int i = 0; i < nbLayers(); i++)
        activation = addOne(elementWise(prod(mLayers[i], activation), sigmoid));

    return activation[0];
}

ublas::matrix<float> createDataset(Corpus &c)
{
    ublas::matrix<float> ds(c.dimension() + 1, c.size());

    for(unsigned int i = 0; i < c.size(); i++)
    {
        for(unsigned int j = 0; j < c.dimension(); j++)
            ds(j,i) = c.elem(i)[j];
        ds(c.dimension(),i) = 1;
    }

    return ds;
}

ublas::vector<float> createTargetVector(Corpus &c)
{
    ublas::vector<float> tv(c.dimension());

    for(unsigned int i = 0; i < c.size(); i++)
        tv[i] = c.elem(i)[c.dimension()];

    return tv;
}

/**
 * MEMO :
 * - dans mLayers[i] :
 *   les neurones de la couche i sont en lignes, les poids de la couche i-1 sont en colonnes
 * - dans ds :
 *   les samples sont en colonnes, les features sont en lignes
 * - dans tv (target vector) :
 *   les features sont en lignes
 * - dans error[i] :
 *   les erreurs des neurones de la couche i sont en lignes, les samples en colonnes
 * - dans delta[i] :
 *   comme mLayers[i]
 */

 //! TODO : debug + regularization

 //! TODO : optimize

//! Backpropagation
std::vector< ublas::matrix<float> > NeuralNetwork::gradient(ublas::matrix<float> &ds, ublas::vector<float> &tv)
{
    std::vector<ublas::matrix<float> > activation(nbLayers() + 1), error(nbLayers() + 1), delta(nbLayers() + 1);
    activation[0] = ds;

    // Forward propagation
    ublas::matrix<float> currAc = addOne(ds);

    for(unsigned int i = 0; i < nbLayers(); i++)
        activation[i+1] = addOne(elementWise(prod(mLayers[i], activation[i]), sigmoid));

    // Backward propagation
    error[nbLayers()] = trans(vecToMat(tv)) - activation[nbLayers()];

    // begin copied code
    delta[nbLayers()-1].resize(mLayers[nbLayers()-1].size1(), mLayers[nbLayers()-1].size2());
    delta[nbLayers()-1] *= 0;
    for(unsigned int j = 0; j < error[nbLayers()].size2(); j++)
    {
        ublas::matrix_range< ublas::matrix<float> >
            errV(error[nbLayers()], ublas::range(0, error[nbLayers()].size1()), ublas::range(j,j+1)),
            actV(activation[nbLayers()-1], ublas::range(0, error[nbLayers()].size1()), ublas::range(j,j+1));

        delta[nbLayers()-1] += prod(errV, trans (actV));
    }
    // end copied code

    for(int i = nbLayers()-1; i >= 1; i--)
    {
        error[i] = element_prod( prod(trans(mLayers[i]), error[i+1]), elementWise(activation[i], d_sigmoid) );
        delta[i-1].resize(mLayers[i-1].size1(), mLayers[i-1].size2());
        delta[i-1] *= 0;
        for(unsigned int j = 0; j < error[i].size2(); j++)
        {
            ublas::matrix_range< ublas::matrix<float> >
                errV(error[i], ublas::range(0, error[i].size1()), ublas::range(j,j+1)),
                actV(activation[i-1], ublas::range(0, error[i].size1()), ublas::range(j,j+1));

            delta[i-1] += prod(errV, trans (actV));
        }
    }

    for(unsigned int i = 0; i < nbLayers(); i++)
         delta[i] /= ds.size2();

    return delta;
}

float NeuralNetwork::gradientDescent(ublas::matrix<float> &ds, ublas::vector<float> &tv, float rate, unsigned int steps)
{
    for(unsigned int i = 0; i < steps; i++)
    {
        std::vector<ublas::matrix<float> > delta = gradient(ds, tv);
        for(unsigned int i = 0; i < nbLayers(); i++)
            mLayers[i] -= rate * delta[i];

    }

    return 0;
}

//! Add a 1 at the end of a vector
ublas::vector<float> NeuralNetwork::addOne(ublas::vector<float> v)
{
    v.resize(v.size() + 1);
    v[v.size()-1] = 1;

    return v;
}

//! Add 1s at the bottom of a matrix
ublas::matrix<float> NeuralNetwork::addOne(ublas::matrix<float> v)
{
    v.resize(v.size1() + 1, v.size2());
    ublas::matrix_row< ublas::matrix<float> > lastRow(v, v.size1()-1);

    std::fill(lastRow.begin(), lastRow.end(), 1);
    return v;
}

//! Apply a function element-wise to a vector
ublas::vector<float> NeuralNetwork::elementWise(ublas::vector<float> v, float (*f)(float))
{
    ublas::vector<float> res = v;
    for(unsigned int i = 0; i < v.size(); i++)
        res[i] = (*f)(v[i]);
    return res;
}

//! Apply a function element-wise to a matrix
ublas::matrix<float> NeuralNetwork::elementWise(ublas::matrix<float> m, float (*f)(float))
{
    // This could be achieved in a smarter way using iterators and the "transform" function
    // std::transform(mat.begin1(), mat.end1(), mat.begin1(), boost::math::tgamma);
    ublas::matrix<float> res = m;
    for(unsigned int i = 0; i < m.size1(); i++)
       for(unsigned int j = 0; j < m.size2(); j++)
        res(i,j) = (*f)(res(i,j));
    return res;
}

//! Create a matrix from a vector
ublas::matrix<float> NeuralNetwork::vecToMat(ublas::vector<float> v)
{
    ublas::matrix<float> res(v.size(), 1);
    ublas::matrix_column< ublas::matrix<float> > col(res,1);
    col = v;

    return res;
}

//! Create a vector from a matrix (unroll it)
ublas::vector<float> NeuralNetwork::unroll(ublas::matrix<float> m)
{
    ublas::vector<float> res(m.size1()*m.size2());
    for(unsigned int i = 0; i < m.size1(); i++)
         for(unsigned int j = 0; j < m.size2(); j++)
              res[i*m.size1()+j] = m(i,j);
    return res;
}

//! Create a vector from a matrix (unroll it)
ublas::matrix<float> roll(ublas::vector<float> v, unsigned int r, unsigned int c)
{
    ublas::matrix<float> res(r,c);
    if(r*c == v.size())
    {
        for(unsigned int i = 0; i < r; i++)
            for(unsigned int j = 0; i < c; j++)
                 res(i,j) = v[i*r + j];
    }
    return res;
}
