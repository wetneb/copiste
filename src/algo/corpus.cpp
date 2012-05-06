/*
 * This file is part of Copiste.
 *
 * Copiste is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 *  Copiste is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Copiste.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "algo/corpus.h"

Corpus::Corpus(std::string file)
{
    mPool = 0;
    mDimension = 0;
    mSize = 0;
    mPoolSize = 0;

    if(file.size())
        load(file);
}

Corpus::Corpus(const Corpus &c, unsigned int keepOnly)
{
    mDimension = std::min(c.dimension(), keepOnly);
    mSize = c.size();
    mPoolSize = c.mPoolSize;
    mPool = new float*[mPoolSize];
    for(int i = 0; i < mSize; i++)
    {
        mPool[i] = new float[mDimension+1];
        for(int j = 0; j < mDimension+1; j++)
            mPool[i][j] = c.elem(i)[j];
    }
}

Corpus::Corpus(int dim)
{
    mPool = 0;
    mDimension = dim;
    mSize = 0;
    mPoolSize = 0;
}

Corpus::~Corpus()
{
    erase();
}

bool Corpus::load(std::string filename, bool verbose)
{
    QDomDocument doc;
    int nbPointsSet = 0, nbCoordsSet = 0;

    // Free everything
    erase();

    // Load the file and parse it
    QFile file(filename.c_str());
    if(!file.open(QIODevice::ReadOnly))
    {
        std::cerr << "Unable to open file : " << filename << std::endl;
        return false;
    }
    doc.setContent(&file);
    file.close();

    // Read the contents
    QDomNode node = doc.documentElement();

    if(node.toElement().tagName() == "corpus")
    {
        mDimension = node.toElement().attribute("dimension", "1").toInt();
        mSize = node.childNodes().size();

        mPool = new float*[mSize]; // deleted in erase()
        mNames.resize(mSize);
        mPoolSize = mSize;

        node = node.firstChild();
        while(!node.isNull() && nbPointsSet < mSize)
        {
            QDomElement elem = node.toElement();

            if(elem.tagName() == "point")
            {
                mPool[nbPointsSet] = new float[mDimension+1]; // deleted in erase()
                mPool[nbPointsSet][0] = node.toElement().attribute("goal", "1").toFloat();

                // in an earlier version, the negative samples were labelled with -1 instead of 0
                if(mPool[nbPointsSet][0] < 0)
                    mPool[nbPointsSet][0] = 0;

                mNames.push_back(node.toElement().attribute("name", "").toStdString());

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

        if(verbose)
            std::cout <<"Loaded "<<mSize<< " points." << std::endl;
    }
    else
    {
        std::cout << "Invalid document : expecting <corpus> markup." << std::endl;
        return false;
    }

    return true;
}

void Corpus::write(std::string fileName)
{
    if(fileName == "")
        return;

    // Open the file
    QFile file(fileName.c_str());
    if(!file.open(QFile::WriteOnly))
        return;

    // Create the document
    QDomDocument doc;
    QDomElement rootNode = doc.createElement("corpus");
    rootNode.setAttribute("dimension", mDimension);
    //rootNode.appendChild(attr);

    for(int iPoint = 0; iPoint < mSize; ++iPoint)
    {
        QDomElement pointNode = doc.createElement("point");

        pointNode.setAttribute("goal", mPool[iPoint][0]);

        for(int iCoord = 0; iCoord < mDimension; ++iCoord)
        {
            QDomElement coordNode = doc.createElement("coord");

            coordNode.setAttribute("value", mPool[iPoint][iCoord+1]);

            pointNode.appendChild(coordNode);
        }

        rootNode.appendChild(pointNode);
    }
    doc.appendChild(rootNode);

    // Write the file
    file.write(doc.toString(4).toAscii());
    file.close();
}

int* randomPermutation(int n)
{
    int* result = new int[n];
    for(int i = 0; i < n; ++i)
        result[i] = -1;

    int remaining = n, currentIndex = 0;
    while(remaining)
    {
        currentIndex = rand() % n;
        while(result[currentIndex] != -1)
            currentIndex = (currentIndex+1) % n;

        result[currentIndex] = --remaining;
    }
    return result;
}

void Corpus::display() const
{
    for(int i = 0; i != mSize; ++i)
    {
        std::cout << "[" << i << "] : ";

        for(int j = 0; j < mDimension+1; ++j)
        {
            std::cout << mPool[i][j] << "  ";
        }
        std::cout << std::endl;
    }
}

std::vector<float> Corpus::bounds() const
{
    std::vector<float> result(mDimension*2,0);

    for(int i = 0; i != mSize; ++i)
        for(int j = 0; j != mDimension; ++j)
        {
            if(i == 0 || mPool[i][j+1] < result[2*j])
                result[2*j] = mPool[i][j+1];
            if(i == 0 || mPool[i][j+1] > result[2*j +1])
                result[2*j +1] = mPool[i][j+1];
        }

    return result;
}

void Corpus::erase(int dimension)
{
    if(mPool)
    {
        for(int i = 0; i != mSize; ++i)
            delete mPool[i];
        delete mPool;
        mPool = 0;
        mPoolSize = 0;
    }
    mSize = 0;
    mDimension = dimension;
}

unsigned int Corpus::size() const
{
    return mSize;
}

unsigned int Corpus::dimension() const
{
    return mDimension;
}

void Corpus::addElem(float* elem, std::string name)
{
    if(mSize >= mPoolSize)
    {
        if(mPoolSize > 0)
        {
            mPoolSize *= 2;
            float** newPool = new float*[mPoolSize];
            for(int i = 0; i < mPoolSize/2; ++i)
                newPool[i] = mPool[i];
            delete mPool;
            mPool = newPool;
        }
        else
        {
            mPoolSize = 1;
            mPool = new float*[mPoolSize];
        }
    }

    if((int)mNames.size() <= mSize)
        mNames.push_back(name);
    else mNames[mSize] = name;

    mPool[mSize] = elem;
    mSize++;
}
