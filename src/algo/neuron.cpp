#include "algo/neuron.h"
#include <cmath>
#include <cstdlib>

neural_value sigmoid(neural_value x)
{
    return (2.0 / (1.0 + exp(-x))) - 1.0;
}

neural_value d_sigmoid(neural_value x)
{
    return 2.0*exp(-x)/((1.0 + exp(-x))*(1.0 + exp(-x)));
}

Neuron::Neuron(string name) : mName(name),
                              mValueCached(false),
                              mErrorCached(false)
{
    // Add the offset parent
    mParents.push_back(0);
    mWeights.push_back(1);
}

Neuron::~Neuron()
{
    for(unsigned int i = 1; i != mParents.size(); ++i)
        mParents[i]->forgetChild(this);
}

void Neuron::load(QDomNode node, const NNetwork* network)
{
    if(!node.isNull() &&
        ((node.toElement().tagName() == "node" && node.toElement().hasAttribute("name"))
        || node.toElement().tagName()=="output"))
    {
        if(node.toElement().hasAttribute("name"))
            mName = node.toElement().attribute("name").toStdString();
        else mName = "output";

        mWeights[0] = node.toElement().attribute("bias", 0).toFloat();

        node = node.firstChild();
        while(!node.isNull())
        {
            if(node.toElement().tagName() == "link" && node.toElement().hasAttribute("parent"))
            {
                addParent(network->getNeuronByName(node.toElement().attribute("parent")),
                            node.toElement().attribute("weight", "1").toFloat());
            }
            node = node.nextSibling();
        }
    }
    else cout << "Warning : wrong neuron definition : " << node.toElement().tagName().toStdString() << endl;
}

// This method will be called only for the output neuron
void Neuron::train(neural_value goal, neural_value rate)
{
    // Get the current value (cached)
    //cout << "G_ERR " << goal - value() << endl;
    mError = goal - value();
    mErrorCached = true;

    // Propagate the error
    spreadError();

    // Compute weights
    computeWeights(rate);
}

neural_value Neuron::error(AbstractNeuron *parent)
{
    if(mErrorCached)
        return (parent ? mError*parentWeight(parent) : mError);

    mError = 0;
    for(unsigned int i = 0; i != mChildren.size(); ++i)
        mError += mChildren[i]->error(this);

    mErrorCached = true;
    return (parent ? mError*parentWeight(parent) : mError);
}

void Neuron::spreadError()
{
    error();

    for(unsigned int i = 1; i != mParents.size(); ++i)
        mParents[i]->spreadError();
}

void Neuron::computeWeights(neural_value rate)
{
    // The value and the error are required
    if(!mErrorCached or !mValueCached)
        return;


    mWeights[0] += d_sigmoid(mValue)* rate * mError;

    // For each weight
    for(unsigned int i = 1; i != mWeights.size(); ++i)
    {
        //cout << "   " << i << " : from "<<mWeights[i]<<" offset " << d_sigmoid(mValue) * mParents[i]->value() * rate * mError << " (" << mParents[i]->name() << " says " << mParents[i]->value() << ")" << endl;
        mWeights[i] += d_sigmoid(mValue) * mParents[i]->value() * rate * mError;
        mParents[i]->computeWeights(rate);
    }
}

void Neuron::randomize(bool spread)
{
    for(unsigned int i = 0; i != mParents.size()+1; ++i)
        mWeights[i] = 2.0*rand()/RAND_MAX - 1;

    if(spread)
        for(unsigned int i = 1; i != mParents.size(); ++i)
            mParents[i]->randomize(true);
}

void Neuron::addParent(AbstractNeuron* parent, neural_value weight)
{
    mParents.push_back(parent);
    mWeights.push_back(weight);
    parent->addChild(this);
}

void Neuron::setParentWeight(AbstractNeuron* parent, neural_value weight)
{
    vector<AbstractNeuron*>::iterator i = find(mParents.begin(), mParents.end(), parent);
    if(i != mParents.end())
        mWeights[distance(mParents.begin(), i)] = weight;
}

neural_value Neuron::parentWeight(AbstractNeuron* parent)
{
    vector<AbstractNeuron*>::iterator i = find(mParents.begin(), mParents.end(), parent);
    if(i != mParents.end())
        return mWeights[distance(mParents.begin(), i)];
    return 0;
}

void Neuron::addChild(AbstractNeuron *child)
{
    mChildren.push_back(child);
}

void Neuron::forgetChild(AbstractNeuron *child)
{
    vector<AbstractNeuron*>::iterator i = find(mParents.begin(), mParents.end(), child);
    if(i != mParents.end())
        mChildren.erase(i);
}

neural_value Neuron::value()
{
    if(mValueCached)
        return mValue;

    neural_value sum = mWeights[0]; // Directly add the offset value

    for(unsigned int i = 1; i < mParents.size(); ++i)
        sum += mParents[i]->value() * mWeights[i];

    mValue = sigmoid(sum);
    mValueCached = true;
    return mValue;
}

void Neuron::clean()
{
    mValueCached = false;
    mErrorCached = false;

    for(unsigned int i = 1; i != mParents.size(); ++i)
        mParents[i]->clean();
}

string Neuron::str() const
{
    stringstream out;
    out << "<" << mName << " : " << (mValueCached ? mValue : 0) << " from :";
    for(unsigned int i = 1; i < mParents.size(); ++i)
        out << "\n+" << mParents[i]->str() << " with weight " << mWeights[i];
    out << ">";
    return out.str();
}

string Neuron::name() const
{
    return mName;
}
