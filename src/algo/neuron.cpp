#include "algo/neuron.h"
#include <cmath>

float sigmoid(float x)
{
    return (1.0 / (1 + exp(-x)));
}

Neuron::Neuron(string name) : mName(name),
                              mValueCached(false),
                              mErrorCached(false)
{
    // Add the offset parent
    mParents.push_back(0);
    mWeights.push_back(1);
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

        node = node.firstChild();
        while(!node.isNull())
        {
            if(node.toElement().tagName() == "link" && node.toElement().hasAttribute("parent"))
            {
                mParents.push_back(network->getNeuronByName(node.toElement().attribute("parent")));
                mWeights.push_back(node.toElement().attribute("weight", "1").toInt());
            }
            node = node.nextSibling();
        }
    }
    else cout << "Warning : wrong neuron definition : " << node.toElement().tagName().toStdString() << endl;
}

// This method will be only called for the output neuron
void Neuron::train(float goal, float rate)
{
    // Get the current value (cached)
    mError = goal - value();
    mErrorCached = true;

    // Propagate the error
    for(unsigned int i = 1; i != mParents.size(); ++i)
        mParents[i]->error();
}

float Neuron::error()
{
    if(mErrorCached)
        return mError;

    mError = 0;
    // For each child
    // TODO

    mErrorCached = true;
    return mError;
}

void Neuron::addParent(AbstractNeuron* parent, float weight)
{
    mParents.push_back(parent);
    mWeights.push_back(weight);
}

void Neuron::removeParent(AbstractNeuron* parent)
{
    unsigned int i = 0;
    while(i < mParents.size() && mParents[i] != parent)
        ++i;
    if(i < mParents.size())
    {
        mParents.erase(mParents.begin()+i);
        mWeights.erase(mWeights.begin()+i);
    }
}

void Neuron::setParentWeight(AbstractNeuron* parent, float weight)
{
    unsigned int i = 0;
    while(i < mParents.size() && mParents[i] != parent)
        ++i;
    if(i < mParents.size())
        mWeights[i] = weight;
}

float Neuron::value()
{
    if(mValueCached)
        return mValue;

    float sum = mWeights[0]; // Directly add the offset value
    for(unsigned int i = 1; i < mParents.size(); ++i)
        sum += mParents[i]->value() * mWeights[i];
    mValue = sigmoid(sum);
    return mValue;
}

void Neuron::clean()
{
    mValueCached = false;
    mErrorCached = false;
}

string Neuron::str() const
{
    stringstream out;
    out << "<" << mName << " : " << (mValueCached ? mValue : 0) << " from :\n";
    for(unsigned int i = 0; i < mParents.size(); ++i)
        out << "+" << mParents[i]->str() << " with weight " << mWeights[i] << "\n";
    return out.str();
}

string Neuron::name() const
{
    return mName;
}
