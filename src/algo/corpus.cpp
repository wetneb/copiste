#include "corpus.h"

bool Corpus::load(string filename)
{
    QDomDocument doc;
    int nbPointsSet = 0, nbCoordsSet = 0;

    QFile file(filename.c_str());
    if(!file.open(QIODevice::ReadOnly))
    {
        cout << "Unable to open file : " << filename << endl;
        return false;
    }
    doc.setContent(&file);
    file.close();

    QDomNode node = doc.documentElement();

    if(node.toElement().tagName() == "corpus")
    {
        mDimension = node.toElement().attribute("dimension", "1").toInt();
        mSize = node.toElement().attribute("size", "100").toInt();

        mPool = new neural_value*[mSize];

        node = node.firstChild();
        while(!node.isNull() && nbPointsSet < mSize)
        {
            QDomElement elem = node.toElement();

            if(elem.tagName() == "point")
            {
                mPool[nbPointsSet] = new neural_value[mDimension];

                nbCoordsSet = 0;
                QDomNode pointNode = node.firstChild();
                while(!pointNode.isNull() && nbCoordsSet < mDimension)
                {
                    mPool[nbPointsSet][nbCoordsSet] = pointNode.toElement().attribute("value", "1").toFloat();
                    nbCoordsSet++;
                }
            }

            nbPointsSet++;
            node = node.nextSibling();
        }
    }
    else
    {
        cout << "Invalid document : expecting <corpus> markup." << endl;
        return false;
    }

    return true;
}

void Corpus::train(NNetwork &network, float learningRate, int maxPasses)
{
    bool errorFound = true;
    vector<int> inputVec;
    inputVec.resize(mDimension);

    for(int i = 0; i != maxPasses*mSize && ((i+1)%mSize || !errorFound); ++i)
    {
        // Restart a new pass
        if((i+1)%mSize == 0)
            errorFound = false;

        // Clean the network
        network.clean();

        // Set up input vector
        for(int coord = 0; coord != mDimension; ++i)
            inputVec[coord] = mPool[i%mSize][coord+1];

        // Check the correctness of the answer
        neural_value answer = network.compute(inputVec);

        // Train
        if(answer*mPool[i%mSize][0] < 0)
        {
            errorFound = true;
            network.train(inputVec, mPool[i%mSize][0], learningRate);
        }
    }
}

void Corpus::display()
{
    for(int i = 0; i != mSize; ++i)
    {
        cout << "[" << i << "] : ";

        for(int j = 0; j != mDimension; ++j)
        {
            cout << mPool[i][j] << "  ";
        }
        cout << endl;
    }
}

unsigned int Corpus::size()
{
    return mSize;
}

unsigned int Corpus::dimension()
{
    return mDimension;
}

