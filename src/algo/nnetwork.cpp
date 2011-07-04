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

NNetwork::NNetwork(string fileName)
{
    if(fileName.size())
        load(fileName);
}

NNetwork::~NNetwork()
{
    clear();
}

void NNetwork::clear()
{
    // \todo delete the neurons !
    // Look at ~Neuron
}

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
                    InputNeuron *neuron = new InputNeuron(subNode.toElement()
                            .attribute("name", QString("elem"+mNeurons.size())).toStdString());
                            // Deleted in ~NNetwork

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
                    Neuron *neuron = new Neuron; // Deleted in ~NNetwork
                    neuron->load(subNode.toElement(), this);
                    mNeurons[QString(neuron->name().c_str())] = neuron;
                    subNode = subNode.nextSibling();
                }
            }
            else if(elem.tagName() == "output")
            {
                Neuron *neuron = new Neuron; // Deleted in ~NNetwork
                neuron->load(node.toElement(), this);
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

void NNetwork::write(string filename)
{
    if(filename == "")
        return;

    QDomDocument doc;
    QDomElement rootElem = doc.createElement("nnetwork");

    // Append inputs
    QDomElement inputsElem = doc.createElement("inputs");
    for(unsigned int i = 0; i != mInputs.size(); ++i)
    {
        QDomElement elem = doc.createElement("node");
        elem.setAttribute("name", mInputs[i]->name().c_str());
        inputsElem.appendChild(elem);
    }
    rootElem.appendChild(inputsElem);

    // Append layers
    QDomElement layerElem = doc.createElement("layer");
    for(QHash<QString, AbstractNeuron*>::iterator i = mNeurons.begin(); i != mNeurons.end(); ++i)
    {
        if(*i != mLastNeuron)
        {
            QDomElement elem = doc.createElement("node");
            elem.setAttribute("name", (*i)->name().c_str());
            if((*i)->write(elem))
                layerElem.appendChild(elem);
        }
    }
    rootElem.appendChild(layerElem);

    // Append output
    QDomElement outputElem = doc.createElement("output");
    mLastNeuron->write(outputElem);
    rootElem.appendChild(outputElem);

    doc.appendChild(rootElem);

    QFile file(filename.c_str());
    if(!file.open(QFile::WriteOnly))
        return;
    file.write(doc.toString(4).toAscii());
    file.close();
}

void NNetwork::randomize()
{
    if(mLastNeuron)
    {
        srand(time(NULL));
        mLastNeuron->randomize(true);
    }
}

void NNetwork::generate(unsigned int dimension, unsigned int depth)
{
    // Clear everything
    clear();

    vector<AbstractNeuron*> lastLayer;

    // Set up inputs
    for(unsigned int i = 0; i < dimension; i++)
    {
        ostringstream buf;
        buf << "Input "<< i+1;
        mInputs.push_back(new InputNeuron(buf.str())); // deleted in clear()
        lastLayer.push_back(mInputs[mInputs.size() - 1]);
    }

    cout << "Generated inputs."<<endl;

    // Set up layers
    for(unsigned int i = 0; i < depth; i++)
    {
        vector<AbstractNeuron*> newLayer;
        for(unsigned int j = 0; j < dimension; j++)
        {
            ostringstream buf;
            buf << "Neuron "<< mNeurons.size() + 1;
            Neuron* n = new Neuron(buf.str());

            for(unsigned int k = 0; k < lastLayer.size(); k++)
                n->addParent(lastLayer[k], 0);

            newLayer.push_back(n);
            mNeurons.insert(buf.str().c_str(), n);
        }

        lastLayer = newLayer;
    }

    cout << "Generated layers."<<endl;

    // Set up output neuron
    Neuron* lastNeuron = new Neuron("Output");
    for(unsigned int j = 0; j < lastLayer.size(); j++)
        lastNeuron->addParent(lastLayer[j], 0);
    mLastNeuron = lastNeuron;

    cout << "Generated output." <<endl;
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

neural_value NNetwork::compute(vector<neural_value> input)
{
    if(!mLastNeuron || input.size() < mInputs.size())
        return 0;

    // Set up inputs
    for(unsigned int i = 0; i != mInputs.size(); ++i)
        mInputs[i]->setValue(input[i]);

    return mLastNeuron->value();
}

neural_value NNetwork::train(vector<neural_value> input, neural_value goal, float rate)
{
    if(!mLastNeuron)
        return 0;

    if(input.size() == mInputs.size())
    {
        neural_value result = compute(input);

        mLastNeuron->train(goal, rate);

        clean();
        return result;
    }
    else cout <<"Warning, bad input vector (size has to be "<<mInputs.size()<<", is "<<input.size()<< ")." << endl;

    return 0;
}

void NNetwork::clean()
{
    mLastNeuron->clean();
}

void NNetwork::display()
{
    cout << mLastNeuron->str() << endl;
}
