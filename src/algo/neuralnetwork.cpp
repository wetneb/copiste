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
    return (1.0 / (1.0 + exp(-x))); // adaptation
}

float d_sigmoid(float x)
{
    return 1.0*exp(-x)/((1.0 + exp(-x))*(1.0 + exp(-x))); // adaptation
}



//! Loads a neural network from a file if specified
NeuralNetwork::NeuralNetwork(std::string file)
{
    if(file != "")
        fromFile(file);
}

//! Creates a fresh network with given input and hidden layer sizes (the output layer is always a single neuron)
void NeuralNetwork::reset(std::vector<int> geometry)
{
    mLayers.resize(geometry.size());

    for(unsigned int i = 1; i < geometry.size(); i++)
        mLayers[i-1].resize(geometry[i], geometry[i-1] + 1, false);
    mLayers[nbLayers() - 1].resize(1, geometry[nbLayers() - 1] + 1);
}

//! Internal usage : for load and save
template<class Archive>
void NeuralNetwork::serialize(Archive & ar, const unsigned int version)
{
    ar & mLayers;
}

//! Loads a network from a file. Returns true if it succeded.
bool NeuralNetwork::fromFile(std::string file)
{
    std::ifstream ifs(file.c_str());
    boost::archive::text_iarchive ar(ifs);
    ar & *this;
    return true;
}

//! Save the current network to a file.
bool NeuralNetwork::toFile(std::string file)
{
    std::ofstream ofs(file.c_str());
    boost::archive::text_oarchive ar(ofs);
    ar & *this;
    return true;
}

void pmatrix(ublas::matrix<float> m, std::string name)
{
    std::cout << name << " : "<< std::endl;
    for(unsigned int i = 0; i < m.size1(); i++)
    {
        for(unsigned int j = 0; j < m.size2(); j++)
            std::cout << m(i,j) << " ";
        std::cout << std::endl;
    }
}

//! Train the network on a corpus
float NeuralNetwork::train(Corpus &c, float rate, float regularization)
{
    ublas::matrix<float> dataset = createDataset(c);
    ublas::vector<float> targetVec = createTargetVector(c);

    std::vector<ublas::matrix<float> > grad;

    for(unsigned int i = 0; i < 10000; i++)
    {
        grad = gradient(dataset, targetVec, regularization);
        /*
        std::vector<ublas::matrix<float> > gradCheck = gradientChecking(dataset, targetVec, regularization, 0.001);

        for(unsigned int k = 0; k < nbLayers(); k++)
        {
            pmatrix(grad[k], "Gradient");
            pmatrix(gradCheck[k], "Check");
        }
        */

        for(unsigned int j = 0; j < nbLayers(); j++)
            mLayers[j] -= rate * grad[j];
    }

    return costFunction(dataset, targetVec);
}

float genRandom(float /*input*/)
{
    return 2.0*rand()/RAND_MAX - 1.0;
}

//! Randomize the weights of the network
void NeuralNetwork::randomize()
{
    srand(time(NULL));
    for(unsigned int i = 0; i < nbLayers(); i++)
        mLayers[i] = elementWise(mLayers[i], genRandom);
}

//! Get the output of the network on a given input
float NeuralNetwork::classify(std::vector<float> input)
{
    //! \todo use the other classify function
    ublas::vector<float> activation(input.size() + 1);
    for(unsigned int i = 0; i < input.size(); i++)
        activation[i] = input[i];
    activation[input.size()] = 1.0;

    for(unsigned int i = 0; i < nbLayers(); i++)
        activation = addOne(elementWise(prod(mLayers[i], activation), sigmoid));

    return activation[0];
}

//! Get the output of the network on a given set of inputs
ublas::matrix<float> NeuralNetwork::classify(ublas::matrix<float> input)
{
    std::vector<ublas::matrix<float> > activation(nbLayers() + 1);
    activation[0] = addOne(input);

    for(unsigned int i = 0; i < nbLayers(); i++)
        activation[i+1] = addOne(elementWise(prod(mLayers[i], activation[i]), sigmoid));

    return removeOnes(activation[nbLayers()]);
}


//! See how well the network does on a given corpus
float NeuralNetwork::accuracy(Corpus &c)
{
    return 0;
}

ublas::matrix<float> NeuralNetwork::createDataset(Corpus &c)
{
    ublas::matrix<float> ds(c.dimension(), c.size());

    for(unsigned int i = 0; i < c.size(); i++)
    {
        for(unsigned int j = 0; j < c.dimension(); j++)
            ds(j,i) = c.elem(i)[j+1];
    }

    return ds;
}

ublas::vector<float> NeuralNetwork::createTargetVector(Corpus &c)
{
    ublas::vector<float> tv(c.size());

    for(unsigned int i = 0; i < c.size(); i++)
        tv[i] = c.elem(i)[0];

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

 void psizes(ublas::matrix<float> m, std::string name)
 {
     std::cout << name << " : "<<m.size1()<< "x"<<m.size2() << std::endl;
 }

//! Backpropagation
std::vector< ublas::matrix<float> > NeuralNetwork::gradient(ublas::matrix<float> &ds, ublas::vector<float> &tv, float regularization)
{
    std::vector<ublas::matrix<float> > activation(nbLayers() + 1), error(nbLayers() + 1), delta(nbLayers() + 1);
    activation[0] = addOne(ds);

    for(unsigned int i = 0; i < nbLayers(); i++)
        activation[i+1] = addOne(elementWise(prod(mLayers[i], activation[i]), sigmoid));

    // Backward propagation
    error[nbLayers()] = removeOnes(activation[nbLayers()]) - trans(vecToMat(tv));

    // \todo begin copied code
    delta[nbLayers()-1].resize(mLayers[nbLayers()-1].size1(), mLayers[nbLayers()-1].size2());
    delta[nbLayers()-1] *= 0;
    for(unsigned int j = 0; j < error[nbLayers()].size2(); j++)
    {
        ublas::matrix_range< ublas::matrix<float> >
            errV = jthCol(error[nbLayers()], j),
            actV = jthCol(activation[nbLayers()-1], j);

        delta[nbLayers()-1] += prod(errV, trans(actV));
    }
    // end copied code

    for(int i = nbLayers()-1; i >= 1; i--)
    {
        error[i] = element_prod( prod(removeOnes(trans(mLayers[i])), error[i+1]), elementWise(removeOnes(activation[i]), d_sigmoid) );
        delta[i-1].resize(mLayers[i-1].size1(), mLayers[i-1].size2());
        delta[i-1] *= 0;
        for(unsigned int j = 0; j < error[i].size2(); j++)
        {
            ublas::matrix_range< ublas::matrix<float> >
                errV = jthCol(error[i], j),
                actV = jthCol(activation[i-1], j);

            delta[i-1] += prod(errV, trans (actV));
        }
    }


    for(unsigned int i = 0; i < nbLayers(); i++)
    {
         delta[i] /= ds.size2();
         for(unsigned int j = 0; j < delta[i].size2() - 1; j++) // we don't regularize the bias term
            jthCol(delta[i],j) += regularization * jthCol(mLayers[i], j);
    }


    return delta;
}

//! Cost function computation
float NeuralNetwork::costFunction(ublas::matrix<float> &ds, ublas::vector<float> &tv, float regularization)
{
    ublas::matrix<float> finalActivations = classify(ds);

    float sum = 0;
    for(unsigned int i = 0; i < finalActivations.size2(); i++)
        sum -= (tv[i] == 1 ? log(finalActivations(0,i)) : log(1.0 - finalActivations(0,i)));

    //! \todo add regularization

    return sum / ds.size2();
}

std::vector< ublas::matrix<float> > NeuralNetwork::gradientChecking(ublas::matrix<float> &ds, ublas::vector<float> &tv, float reg, float epsilon)
{
    std::vector<ublas::matrix<float> > grad(nbLayers());

    for(unsigned int k = 0; k < nbLayers(); k++)
    {
        grad[k].resize(mLayers[k].size1(), mLayers[k].size2());
        for(unsigned int i = 0; i < mLayers[k].size1(); i++)
        {
            for(unsigned int j = 0; j < mLayers[k].size2(); j++)
            {
                mLayers[k](i,j) += epsilon;
                float cost_plus = costFunction(ds,tv,reg);
                mLayers[k](i,j) -= 2*epsilon;
                float cost_minus = costFunction(ds,tv,reg);
                mLayers[k](i,j) += epsilon;
                grad[k](i,j) = (cost_plus - cost_minus) / (2*epsilon);
            }
        }
    }

    return grad;
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

ublas::matrix_range<ublas::matrix<float> > NeuralNetwork::removeOnes(ublas::matrix<float> m)
{
    return ublas::matrix_range<ublas::matrix<float> >(m, ublas::range(0,m.size1()-1), ublas::range(0,m.size2()));
}

ublas::matrix_range<ublas::matrix<float> > NeuralNetwork::jthCol(ublas::matrix<float> m, size_t j)
{
    return ublas::matrix_range<ublas::matrix<float> >(m, ublas::range(0,m.size1()), ublas::range(j,j+1));
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
    ublas::matrix_column< ublas::matrix<float> > col(res,0);
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
