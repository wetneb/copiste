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
    if(file.size())
        load(file);
}
    
Corpus::Corpus(unsigned int dim)
{
    mSamples.resize(dim, 0);
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

    // Old version of the loading code, kept for compatibility
    if(node.toElement().tagName() == "corpus" && node.toElement().attribute("version", "1") == "1")
    {
        int dimension = node.toElement().attribute("dimension", "1").toInt();
        int size = node.childNodes().size();

        mSamples.resize(dimension, size);
        mTargetClass.resize(size);
        mNames.resize(size);
        
        node = node.firstChild();
        while(!node.isNull())
        {
            QDomElement elem = node.toElement();
            if(elem.tagName() == "point")
            {
                int tgcl = node.toElement().attribute("goal", "1").toInt();
                // in an earlier version, the negative samples were labelled with -1 instead of 0
                if(tgcl < 0)
                    tgcl = 0;
                mTargetClass[nbPointsSet] = tgcl;

                mNames[nbPointsSet] = node.toElement().attribute("name", "").toStdString();

                nbCoordsSet = 0;
                QDomNode pointNode = node.firstChild();
                while(!pointNode.isNull() && nbCoordsSet < dimension && pointNode.toElement().tagName() == "coord")
                {
                    mSamples(nbCoordsSet, nbPointsSet) = pointNode.toElement().attribute("value", "1").toFloat();

                    nbCoordsSet++;
                    pointNode = pointNode.nextSibling();
                }

                nbPointsSet++;
            }

            node = node.nextSibling();
        }

        // resize down to the actual number of points set
        mSamples.resize(dimension, nbPointsSet);
        mTargetClass.resize(nbPointsSet);
        mNames.resize(nbPointsSet);

        if(verbose)
            std::cout <<"Loaded "<<nbPointsSet<< " points." << std::endl;
    }
    // Current version of the loading code
    else if(node.toElement().tagName() == "corpus" && node.toElement().attribute("version", "1") == "2")
    {
        QDomElement rootElem = node.toElement();

        int size = rootElem.childNodes().length();
        int dimension = rootElem.attribute("features", "0").toInt();
        mSamples.resize(size, dimension);
        mTargetClass.resize(size);
        mNames.resize(size);

        QDomNode currNode = rootElem.firstChild();
      
        int nbPointsSet = 0;
        while(!currNode.isNull())
        {
            QDomElement elem = currNode.toElement();
            if(elem.tagName() != "point")
            {
                std::cerr << filename << " : Ignored markup <"
                    << elem.tagName().toStdString() << ">." << std::endl;
            }
            else
            {
                mTargetClass[nbPointsSet] = elem.attribute("class", "0").toInt();
                mNames[nbPointsSet] = node.toElement().attribute("name", "").toStdString();
                std::istringstream stream(elem.text().toStdString());
                try
                {
                    ublas::vector<double> fp;
                    boost::archive::text_iarchive ar(stream);
                    ar & fp;
                    ublas::matrix_column<ublas::matrix<double> > mc(mSamples, nbPointsSet);
                    mc = fp;
                }
                catch(boost::archive::archive_exception ex)
                {
                    std::cerr << filename << " : Error, invalid fingerprint : " << ex.what() << std::endl;
                    return false;
                }

                nbPointsSet++;
            }
        }

        // Reduce the size of the arrays (some fingerprints may be invalid)
        mSamples.resize(nbPointsSet, dimension);
        mTargetClass.resize(nbPointsSet);
    }
    else
    {
        std::cerr << filename << " : Invalid document : expected <corpus> markup." << std::endl;
        return false;
    }

    return true;
}

void Corpus::write(std::string filename)
{
    QFile file(filename.c_str());
    if(!file.open(QFile::WriteOnly))
        std::cerr << filename << " : Unable to open the file." << std::endl;

    QDomDocument doc;
    
    QDomElement rootNode = doc.createElement("corpus");
    rootNode.setAttribute("dimension", dimension());

    for(unsigned int i = 0; i < size(); i++)
    {
        QDomElement fpNode = doc.createElement("point");

        fpNode.setAttribute("class", mTargetClass[i]);
        fpNode.setAttribute("name", mNames[i].c_str());
        ublas::vector<double> vec = point(i);
        std::ostringstream stream;
        boost::archive::text_oarchive ar(stream);
        ar & vec;        
        fpNode.appendChild(doc.createTextNode(stream.str().c_str()));
        
        rootNode.appendChild(fpNode);
    }

    doc.appendChild(rootNode);

    // 4 is the indentation
    file.write(doc.toString(4).toAscii());
    file.close();
}

void Corpus::display() const
{
    for(unsigned int i = 0; i != size(); ++i)
    {
        std::cout << "[" << i << "] : |";
        std::cout << mTargetClass[i] << "| ";

        for(unsigned int j = 0; j < dimension(); ++j)
        {
            std::cout << mSamples(j, i) << "  ";
        }
        std::cout << std::endl;
    }
}

std::vector<double> Corpus::bounds() const
{
    std::vector<double> result(dimension()*2,0);

    for(unsigned int i = 0; i != dimension(); ++i)
        for(unsigned int j = 0; j != size(); ++j)
        {
            if(i == 0 || mSamples(i, j) < result[2*i])
                result[2*i] = mSamples(i, j);
            if(i == 0 || mSamples(i, j) > result[2*i +1])
                result[2*i +1] = mSamples(i, j);
        }

    return result;
}

void Corpus::erase(int dimension)
{
    mSamples.resize(dimension, 0);
    mTargetClass.clear();
    mNames.clear();
}

unsigned int Corpus::size() const
{
    return mSamples.size2();
}

unsigned int Corpus::dimension() const
{
    return mSamples.size1();
}

// DEPRECATED
void Corpus::addElem(double* elem, std::string name)
{
    ublas::vector<double> vec(dimension());
    for(unsigned int i = 0; i < dimension(); i++)
        vec[i] = elem[i+1];
    addSample(vec, vec[0], name);
}

void Corpus::addSample(ublas::vector<double> elem, int targetClass, std::string name)
{
    mSamples.resize(dimension(), size() + 1);
    ublas::matrix_column<ublas::matrix<double> > mc(mSamples, size()-1);
    mc = elem;
    mTargetClass.push_back(targetClass);
    mNames.push_back(name);
}

ublas::matrix<int> Corpus::asFingerprints() 
{
    ublas::matrix<int> res(mSamples.size1(), mSamples.size2());
    ublas::matrix<double>::iterator1 iterSource = mSamples.begin1();
    ublas::matrix<int>::iterator1 iterDest = res.begin1();
    while(iterSource != mSamples.end1())
    {
        (*iterSource) = *iterDest;
        iterSource++;
        iterDest++;
    }
    return res;    
}

ublas::vector<double> Corpus::point(unsigned int idx)
{
    if(idx < size())
    {
        ublas::matrix_column< ublas::matrix<double> > mc(mSamples, idx);
        return mc;
    }
    std::cerr << "Corpus::point(index) : index out of bounds." << std::endl;
    return ublas::vector<double>(dimension());
}

ublas::matrix<double> Corpus::asDataset()
{
    return mSamples;
}

int Corpus::getClass(unsigned int index)
{
    if(index < mTargetClass.size())
        return mTargetClass[index];
    std::cerr << "Corpus::getClass(index) : index out of bounds." << std::endl;
    return 0;
}

