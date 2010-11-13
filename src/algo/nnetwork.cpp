#include "algo/nnetwork.h"

#include <iostream>

#include <QDomDocument>
#include <QFile>
#include <QBuffer>
#include <QByteArray>
#include <QXmlSerializer>
#include <QXmlResultItems>
#include <QStringList>

using namespace std;

bool NNetwork::load(string fileName)
{
    QDomDocument doc;

    QFile file(fileName.c_str());
    if(!file.open(QIODevice::ReadOnly))
    {
        cout << "Unable to open file : " << fileName << endl;
        return false;
    }
    doc.setContent(&file);
    file.close();

    QDomNode node = doc.documentElement();

    if(node.toElement().tagName() == "nnetwork")
    {
        node = node.firstChild();
        while(!node.isNull())
        {
            QDomElement elem = node.toElement();

            if(elem.tagName() == "inputs")
            {
                QDomNode subNode = node.firstChild();
                while(!subNode.isNull())
                {
                    InputNeuron *neuron = new InputNeuron(subNode.toElement().attribute("name", QString("elem"+mNeurons.size())).toStdString());
                    mInputs.push_back(neuron);
                    mNeurons[QString(neuron->name().c_str())] = neuron;
                    subNode = subNode.nextSibling();
                }
            }
            else if(elem.tagName() == "layer")
            {
                QDomNode subNode = node.firstChild();
                while(!subNode.isNull() && subNode.toElement().tagName() == "node")
                {
                    Neuron *neuron = new Neuron;
                    neuron->load(subNode, this);
                    mNeurons[QString(neuron->name().c_str())] = neuron;
                    subNode = subNode.nextSibling();
                }
            }
            else if(elem.tagName() == "output")
            {
                Neuron *neuron = new Neuron;
                neuron->load(node, this);
                mLastNeuron = neuron;
            }
            node = node.nextSibling();
        }
    }
    else
    {
        cout << "Invalid document : must have <nnetwork> as root element, has "<<node.toElement().tagName().toStdString()<<"." << endl;
        return false;
    }
    return true;
}

AbstractNeuron* NNetwork::getNeuronByName(string name) const
{
    return mNeurons[QString(name.c_str())];
}

AbstractNeuron* NNetwork::getNeuronByName(QString name) const
{
    return mNeurons[name];
}

unsigned int NNetwork::addInput(InputNeuron* input)
{
    mInputs.push_back(input);
    return mInputs.size()-1;
}

void NNetwork::removeInput(unsigned int id)
{
    if(id < mInputs.size())
        mInputs.erase(mInputs.begin()+id);
}

float NNetwork::compute(vector<int> input)
{
    if(!mLastNeuron)
        return 0;
    return mLastNeuron->value();
}

float NNetwork::train(vector<int> input, float goal, float rate)
{
    if(!mLastNeuron)
        return 0;
    float result = mLastNeuron->value();

    mLastNeuron->train(goal, rate);

    return result;
}

void NNetwork::display()
{
    for(QHash<QString, AbstractNeuron*>::iterator iter = mNeurons.begin(); iter != mNeurons.end(); ++iter)
        cout << (*iter)->str() << endl;
}
