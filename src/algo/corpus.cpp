#include "corpus.h"

Corpus::Corpus(string file)
{
    mPool = 0;
    mDimension = 0;
    mSize = 0;

    if(file.size())
        load(file);
}

Corpus::~Corpus()
{
    erase();
}

bool Corpus::load(string filename)
{
    QDomDocument doc;
    int nbPointsSet = 0, nbCoordsSet = 0;

    // Free everything
    erase();

    // Load the file and parse it
    QFile file(filename.c_str());
    if(!file.open(QIODevice::ReadOnly))
    {
        cout << "Unable to open file : " << filename << endl;
        return false;
    }
    doc.setContent(&file);
    file.close();

    // Read the contents
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
                mPool[nbPointsSet] = new neural_value[mDimension+1];
                mPool[nbPointsSet][0] = node.toElement().attribute("goal", "1").toFloat();

                nbCoordsSet = 0;
                QDomNode pointNode = node.firstChild();
                while(!pointNode.isNull() && nbCoordsSet < mDimension && pointNode.toElement().tagName() == "coord")
                {
                    mPool[nbPointsSet][nbCoordsSet+1] = pointNode.toElement().attribute("value", "1").toFloat();

                    nbCoordsSet++;
                    pointNode = pointNode.nextSibling();
                }

                nbPointsSet++;
            }

            node = node.nextSibling();
        }

        mSize = nbPointsSet;
        cout <<"Loaded "<<mSize<< " points." << endl;
    }
    else
    {
        cout << "Invalid document : expecting <corpus> markup." << endl;
        return false;
    }

    return true;
}

int Corpus::train(NNetwork &network, float learningRate, int maxPasses)
{
    bool errorFound = false;
    vector<neural_value> inputVec;
    inputVec.resize(mDimension);

    int i;
    for(i = 0; i != maxPasses*mSize && ((i+1)%mSize || errorFound); ++i)
    {
        // Restart a new pass
        if((i+1)%mSize == 0)
            errorFound = false;

        // Clean the network
        network.clean();

        // Set up input vector
        for(int coord = 0; coord != mDimension; ++coord)
            inputVec[coord] = mPool[i%mSize][coord+1];

        // Check the correctness of the answer
        neural_value answer = network.compute(inputVec);

        // Train
        if((mPool[i%mSize][0])*answer <= 0)
        {
            //cout <<"["<<inputVec[0]<<"]["<<inputVec[1]<<"] Goal = " << mPool[i%mSize][0] << ", result = "<< answer << endl;
            errorFound = true;
            network.train(inputVec, mPool[i%mSize][0], learningRate);
        }
    }
    return i;
}

float Corpus::compliance(NNetwork &network)
{
    int errorsFound = 0;
    vector<neural_value> inputVec;
    inputVec.resize(mDimension);

    for(int i = 0; i != mSize; ++i)
    {
        // Clean the network
        network.clean();

        // Set up input vector
        cout << "Testing ";
        for(int coord = 0; coord != mDimension; ++coord)
        {
            inputVec[coord] = mPool[i][coord+1];
            cout << "["<< inputVec[coord] << "]";
        }
        cout << " : Result : " << network.compute(inputVec)*mPool[i][0] << endl;

        // Compute
        if(network.compute(inputVec)*mPool[i][0] <= 0)
            errorsFound++;
    }
    return ((float)errorsFound)/mSize;
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

void Corpus::erase()
{
    if(mPool)
    {
        for(int i = 0; i != mSize; ++i)
            delete mPool[i];
        delete mPool;
        mSize = 0;
        mDimension = 0;
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

