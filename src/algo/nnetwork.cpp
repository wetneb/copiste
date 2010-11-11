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
                    InputPerceptron *perceptron = new InputPerceptron(subNode.toElement().attribute("name", QString("elem"+mPerceptrons.size())).toStdString());
                    mInputs.push_back(perceptron);
                    mPerceptrons[QString(perceptron->name().c_str())] = perceptron;
                    subNode = subNode.nextSibling();
                }
            }
            else if(elem.tagName() == "layer")
            {
                QDomNode subNode = node.firstChild();
                while(!subNode.isNull() && subNode.toElement().tagName() == "node")
                {
                    Perceptron *perceptron = new Perceptron;
                    perceptron->load(subNode, this);
                    mPerceptrons[QString(perceptron->name().c_str())] = perceptron;
                    subNode = subNode.nextSibling();
                }
            }
            else if(elem.tagName() == "output")
            {
                Perceptron *perceptron = new Perceptron;
                perceptron->load(node, this);
                mLastPerceptron = perceptron;
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

AbstractPerceptron* NNetwork::getPerceptronByName(string name) const
{
    return mPerceptrons[QString(name.c_str())];
}

AbstractPerceptron* NNetwork::getPerceptronByName(QString name) const
{
    return mPerceptrons[name];
}

unsigned int NNetwork::addInput(InputPerceptron* input)
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
    if(!mLastPerceptron)
        return 0;
    return mLastPerceptron->value();
}

float NNetwork::train(vector<int> input, float goal, float variation)
{
    if(!mLastPerceptron)
        return 0;
    float result = mLastPerceptron->value();

    // TBC

    return result;
}

void NNetwork::display()
{
    for(QHash<QString, AbstractPerceptron*>::iterator iter = mPerceptrons.begin(); iter != mPerceptrons.end(); ++iter)
        cout << (*iter)->str() << endl;
}
