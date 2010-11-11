#include "algo/perceptron.h"
#include <cmath>

float sigmoid(float x)
{
    return (1.0 / (1 + exp(-x)));
}

void Perceptron::load(QDomNode node, const NNetwork* network)
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
                mParents.push_back(network->getPerceptronByName(node.toElement().attribute("parent")));
                mWeights.push_back(node.toElement().attribute("weight", "1").toInt());
            }
            node = node.nextSibling();
        }
    }
    else cout << "Warning : wrong perceptron definition : " << node.toElement().tagName().toStdString() << endl;
}

void Perceptron::addParent(AbstractPerceptron* parent, float weight)
{
    mParents.push_back(parent);
    mWeights.push_back(weight);
}

void Perceptron::removeParent(AbstractPerceptron* parent)
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

void Perceptron::setParentWeight(AbstractPerceptron* parent, float weight)
{
    unsigned int i = 0;
    while(i < mParents.size() && mParents[i] != parent)
        ++i;
    if(i < mParents.size())
        mWeights[i] = weight;
}

float Perceptron::value() const
{
    float sum = 0;
    for(unsigned int i = 0; i < mParents.size(); ++i)
        sum += mParents[i]->value() * mWeights[i];
    return sigmoid(sum);
}

string Perceptron::str() const
{
    stringstream out;
    out << "<" << mName << " : " << value() << " from :\n";
    for(unsigned int i = 0; i < mParents.size(); ++i)
        out << "+" << mParents[i]->str() << " with weight " << mWeights[i] << "\n";
    return out.str();
}

string Perceptron::name() const
{
    return mName;
}
