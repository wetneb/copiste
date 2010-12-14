#include "corpus.h"

bool Corpus::load(string filename)
{
    bool dimensionIsSet = false;
    QDomDocument doc;

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
        node = node.firstChild();
        while(!node.isNull())
        {
            QDomElement elem = node.toElement();

            //if() //TBC
            /*******************
             * TO BE CONTINUED *
             *******************/
        }
    }
    else
    {
        cout << "Invalid document : expecting <corpus> markup." << endl;
        return false;
    }

    return true;
}

void Corpus::train(NNetwork &network)
{

}

void Corpus::display()
{
    for(unsigned int i = 0; i != mSize; ++i)
    {
        cout << "[" << i << "] : ";

        for(unsigned int j = 0; j != mDimension; ++j)
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

