#include "corpus.h"

/**
class Corpus
{
    public:
        //! Loads the corpus from an XML file
        bool load(string file);

        //! Trains a given network to match the corpus
        void train(NNetwork &network);

        //! Writes to the standard output the set of elements
        void display();

        //! Returns the number of elements (i.e. vectors) contained in the corpus
        unsigned int size();

        //! Returns the number of inputs of each training example (i.e. the number of coordinates of each vector)
        unsigned int dimension();

        //! Returns the elem pointed by the given index
        float* elem(unsigned int index);

    private:
        float** mPool;
        int mSize;
        int mDimension;
};
**/

bool Corpus::load(string file)
{
    bool dimensionIsSet = false;
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
    for(unsigned int i = 0; i != size; ++i)
    {
        cout << "[" << i << "] : ";

        for(unsigned int j = 0; j != dimension; ++j)
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

