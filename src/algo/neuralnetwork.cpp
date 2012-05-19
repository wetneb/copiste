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

 double sigmoid(double x) // This is actually the hyperbolic tangent
{
    if(x > 45)
	    return 1.0;
    else if(x < - 45)
	    return 0;
    return (1.0 / (1.0 + exp(-x))); 
}
   
double d_sigmoid(double x)
{
    if(x > 45)
	    return 0;
    else if(x < -45)
	    return 1;
    double val = 1.0 + exp(-x);
    return (val - 1)/(val*val); 
}


void pmatrix(ublas::matrix<double> m, std::string name);

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
    bool status = false;

    if(!ifs.is_open())
        std::cerr << "Unable to open the file " << file << std::endl;
    else
    {
	    try
	    {
            boost::archive::text_iarchive ar(ifs);
            ar & *this;
	        status = true;
	    }
	    catch(boost::archive::archive_exception ex)
	    {	
	        std::cerr << "Error while loading the neural network from the file " << file << " : " << ex.what() << std::endl;
	    }
    }
    return status;
}

//! Save the current network to a file.
bool NeuralNetwork::toFile(std::string file)
{
    std::ofstream ofs(file.c_str());
    bool status = false;

    if(!ofs.is_open())
	    std::cerr << "Unable to write to the file " << file << std::endl;
    else
    {
        try
        {
    	    boost::archive::text_oarchive ar(ofs);
    	    ar & *this;
	        status = true;
        }
        catch(boost::archive::archive_exception ex)
	    {
	        std::cerr << "Error while writing the neural network to the file " << file << " : " << ex.what() << std::endl;
	    }
    }
    
    return status;
}

void pmatrix(ublas::matrix<double> m, std::string name)
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
double NeuralNetwork::train(Corpus &c, double rate, double regularization, int nbIter, bool debug)
{
    if(c.dimension() != mLayers[0].size2() - 1)
    {
	    std::cerr << "Error : the corpus and the network don't have the same dimension." << std::endl;
	    return 0;
    }

    ublas::matrix<double> dataset = createDataset(c);
    ublas::vector<double> targetVec = createTargetVector(c);

    std::vector<ublas::matrix<double> > grad;

    for(int i = 0; i < nbIter; i++)
    {
        grad = gradient(dataset, targetVec, regularization);
        
        if(debug)
	    {
	       	std::vector<ublas::matrix<double> > gradCheck =
		    gradientChecking(dataset, targetVec, regularization, 0.000001);
            // the last value is epsilon, used to compute the slope

        	for(unsigned int k = 0; k < nbLayers(); k++)
        	{
            	pmatrix(grad[k], "Gradient");
            	pmatrix(gradCheck[k], "Check");
        	}
	    }

        for(unsigned int j = 0; j < nbLayers(); j++)
            mLayers[j] -= rate * grad[j];
    }

    return 0; // costFunction(dataset, targetVec, regularization);
}

double genRandom(double /*input*/)
{
    return 2.0*rand()/RAND_MAX - 1.0;
}

//! Return the input dimension of the network
unsigned int NeuralNetwork::dimension()
{
    if(mLayers.size())
        return (mLayers[0].size2() - 1);
    else
        return 0;
}

//! Randomize the weights of the network
void NeuralNetwork::randomize()
{
    srand(time(NULL));
    for(unsigned int i = 0; i < nbLayers(); i++)
        mLayers[i] = elementWise(mLayers[i], genRandom);
}

//! Get the output of the network on a given input
double NeuralNetwork::classify(std::vector<double> input)
{
    double result = 0;
    if(input.size() == dimension())
    { 
        ublas::matrix<double> newInput(input.size(),1);
        for(unsigned int i = 0; i < input.size(); i++)
	        newInput(i,0) = input[i];
        ublas::matrix<double> activation = classify(newInput);
     
        result = activation(0,0);
    }
    else
	    std::cerr << "Warning, trying to classify a sample with a wrong dimension." << std::endl;
    return result;
}

//! Get the output of the network on a given set of inputs
ublas::matrix<double> NeuralNetwork::classify(ublas::matrix<double> input)
{
    std::vector<ublas::matrix<double> > activation(nbLayers() + 1);
    activation[0] = addOne(input);

    for(unsigned int i = 0; i < nbLayers(); i++)
        activation[i+1] = addOne(elementWise(prod(mLayers[i], activation[i]), sigmoid));

    return removeOnes(activation[nbLayers()]);
}


double NeuralNetwork::accuracy(Corpus &c)
{
    ublas::matrix<double> ds = createDataset(c);
    ublas::matrix<double> response = classify(ds);
    ublas::vector<double> tv = createTargetVector(c);
    int nbCorrect = 0;
    for(unsigned int i = 0; i < c.size(); i++)
	   if((tv[i] - 0.5)*(response(0,i) - 0.5) > 0)
		   nbCorrect++;

    return ((double)nbCorrect)/c.size();
}

ublas::matrix<double> NeuralNetwork::createDataset(Corpus &c)
{
    ublas::matrix<double> ds(c.dimension(), c.size());

    for(unsigned int i = 0; i < c.size(); i++)
    {
        for(unsigned int j = 0; j < c.dimension(); j++)
            ds(j,i) = c.elem(i)[j+1];
    }

    return ds;
}

ublas::vector<double> NeuralNetwork::createTargetVector(Corpus &c)
{
    ublas::vector<double> tv(c.size());

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


 //! TODO : optimize

// !TODO delete this function
 void psizes(ublas::matrix<double> m, std::string name)
 {
     std::cout << name << " : "<<m.size1()<< "x"<<m.size2() << std::endl;
 }

//! Backpropagation
std::vector< ublas::matrix<double> > NeuralNetwork::gradient(ublas::matrix<double> &ds,
		                                        ublas::vector<double> &tv, double regularization)
{
    std::vector<ublas::matrix<double> > activation(nbLayers() + 1), error(nbLayers() + 1), delta(nbLayers());
    activation[0] = addOne(ds);

    // Forward propagation
    for(unsigned int i = 0; i < nbLayers(); i++)
        activation[i+1] = addOne(elementWise(prod(mLayers[i], activation[i]), sigmoid));

    // Backward propagation
    for(unsigned int i = nbLayers(); i >= 1; i--) // Loop through the layers (backward)
    {
	    if(i == nbLayers())
		    error[i] = removeOnes(activation[i]) - trans(vecToMat(tv));
	    else
        	error[i] = element_prod(prod(removeOnes(trans(mLayers[i])), error[i+1]),
			                 elementWise(removeOnes(activation[i]), d_sigmoid));

        delta[i-1] = ublas::zero_matrix<double>(mLayers[i-1].size1(), mLayers[i-1].size2());
	
        for(unsigned int j = 0; j < error[i].size2(); j++) // Loop through the samples of the dataset
            delta[i-1] += prod(jthCol(error[i],j), trans (jthCol(activation[i-1],j)));
    }


    for(unsigned int i = 0; i < nbLayers(); i++) // we don't regularize the input layer (en fait là si \todo)
    {
         for(unsigned int j = 0; j < delta[i].size2() - 1; j++) // we don't regularize the bias term
	        for(unsigned int k = 0; k < delta[i].size1(); k++)
		        delta[i](k,j) += regularization * mLayers[i](k,j);
	     // Attention : si on change i = 0 au dessus, il faut faire delta[0] /= ds.size2() ailleurs !!!
         delta[i] /= ds.size2();
    }

    return delta;
}

//! Cost function computation
double NeuralNetwork::costFunction(ublas::matrix<double> &ds, ublas::vector<double> &tv, double regularization)
{
    ublas::matrix<double> finalActivations = classify(ds);

    double sum = 0;
    for(unsigned int i = 0; i < finalActivations.size2(); i++)
        sum -= (tv[i] > 0.5 ? log(finalActivations(0,i)) : log(1.0 - finalActivations(0,i)));

    for(unsigned int i = 0; i < nbLayers(); i++) // we don't regularize the input layer (en fait là si \todo)
	    for(unsigned int j = 0; j < mLayers[i].size1(); j++)
		    for(unsigned int k = 0; k < mLayers[i].size2() - 1; k++) // we don't regularize the bias term
			    sum += regularization * 0.5 * pow(mLayers[i](j,k), 2);
    
    sum /= ds.size2();

    return sum;
}

std::vector< ublas::matrix<double> > NeuralNetwork::gradientChecking(ublas::matrix<double> &ds, ublas::vector<double> &tv, double reg, double epsilon)
{
    std::vector<ublas::matrix<double> > grad(nbLayers());

    for(unsigned int k = 0; k < nbLayers(); k++)
    {
        grad[k].resize(mLayers[k].size1(), mLayers[k].size2());
        for(unsigned int i = 0; i < mLayers[k].size1(); i++)
        {
            for(unsigned int j = 0; j < mLayers[k].size2(); j++)
            {
                mLayers[k](i,j) += epsilon;
                double cost_plus = costFunction(ds,tv,reg);
                mLayers[k](i,j) -= 2*epsilon;
                double cost_minus = costFunction(ds,tv,reg);
                mLayers[k](i,j) += epsilon;
                grad[k](i,j) = (cost_plus - cost_minus) / (2*epsilon);
            }
        }
    }

    return grad;
}

//! Add a 1 at the end of a vector
ublas::vector<double> NeuralNetwork::addOne(ublas::vector<double> v)
{
    v.resize(v.size() + 1);
    v[v.size()-1] = 1;

    return v;
}

//! Add 1s at the bottom of a matrix
ublas::matrix<double> NeuralNetwork::addOne(ublas::matrix<double> v)
{
    v.resize(v.size1() + 1, v.size2());
    ublas::matrix_row< ublas::matrix<double> > lastRow(v, v.size1()-1);

    std::fill(lastRow.begin(), lastRow.end(), 1);
    return v;
}

ublas::matrix_range<ublas::matrix<double> > NeuralNetwork::removeOnes(ublas::matrix<double> m)
{
    return ublas::matrix_range<ublas::matrix<double> >(m, ublas::range(0,m.size1()-1), ublas::range(0,m.size2()));
}

ublas::matrix_range<ublas::matrix<double> > NeuralNetwork::jthCol(ublas::matrix<double> m, size_t j)
{
    return ublas::matrix_range<ublas::matrix<double> >(m, ublas::range(0,m.size1()), ublas::range(j,j+1));
}

//! Apply a function element-wise to a vector
ublas::vector<double> NeuralNetwork::elementWise(ublas::vector<double> v, double (*f)(double))
{
    ublas::vector<double> res = v;
    for(unsigned int i = 0; i < v.size(); i++)
        res[i] = (*f)(v[i]);
    return res;
}

//! Apply a function element-wise to a matrix
ublas::matrix<double> NeuralNetwork::elementWise(ublas::matrix<double> m, double (*f)(double))
{
    // This could be achieved in a smarter way using iterators and the "transform" function
    // std::transform(mat.begin1(), mat.end1(), mat.begin1(), boost::math::tgamma);
    ublas::matrix<double> res = m;
    for(unsigned int i = 0; i < m.size1(); i++)
        for(unsigned int j = 0; j < m.size2(); j++)
            res(i,j) = (*f)(res(i,j));
    return res;
}

//! Create a matrix from a vector
ublas::matrix<double> NeuralNetwork::vecToMat(ublas::vector<double> v)
{
    ublas::matrix<double> res(v.size(), 1);
    ublas::matrix_column< ublas::matrix<double> > col(res,0);
    col = v;
    return res;
}

//! Create a vector from a matrix (unroll it)
ublas::vector<double> NeuralNetwork::unroll(ublas::matrix<double> m)
{
    ublas::vector<double> res(m.size1()*m.size2());
    for(unsigned int i = 0; i < m.size1(); i++)
         for(unsigned int j = 0; j < m.size2(); j++)
              res[i*m.size1()+j] = m(i,j);
    return res;
}

//! Create a vector from a matrix (unroll it)
ublas::matrix<double> roll(ublas::vector<double> v, unsigned int r, unsigned int c)
{
    ublas::matrix<double> res(r,c);
    if(r*c == v.size())
    {
        for(unsigned int i = 0; i < r; i++)
            for(unsigned int j = 0; i < c; j++)
                 res(i,j) = v[i*r + j];
    }
    return res;
}
