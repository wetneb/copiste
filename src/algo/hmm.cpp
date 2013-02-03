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

#include "algo/hmm.h"

using namespace boost::numeric;

HMM::HMM(bool training) : mTraining(training), mNbStates(0),
            mMatrix(0), mNbFpSeen(0), mCurrentState(0), mObserver(0)
{
    ;
}

HMM::~HMM()
{
    if(mMatrix)
    {
        for(int i = 0; i < mNbStates; i++)
            if(mMatrix[i])
                delete mMatrix[i];
        delete mMatrix;
    }
}

void HMM::erase(int n)
{
    mNbStates = n;
    mNullVector.resize(n);
    mMatrix = new int*[n];
    for(int i = 0; i < n; i++)
    {
        mNullVector[i] = 0;
        mMatrix[i] = new int[n];
        for(int j = 0; j < n; j++)
            mMatrix[i][j] = 0;
    }
    mNbFpSeen = 0;
}

bool HMM::load(string filename)
{
    QDomDocument doc;

    QFile file(filename.c_str());
    if(!file.open(QIODevice::ReadOnly))
    {
        std::cerr << "Unable to open the file : \""
         << filename << std::endl;
        return false;
    }   
    doc.setContent(&file);
    file.close();

    QDomNode node = doc.documentElement();

    if(node.toElement().tagName() == "model")
    {
        QDomElement rootElem = node.toElement();
        int nbstates = rootElem.attribute("states", "0").toInt();
        if(!rootElem.hasAttribute("states"))
        {
            std::cerr << filename <<
            " : Number of states not specified."
            << std::endl;
        }
        else if(nbstates <= 0)
        {
            std::cerr << filename <<
             " : Invalid model : must have a positive number of states."
             << std::endl;
        }

        erase(nbstates);

        node = node.firstChild();
        while(!node.isNull())
        {
            QDomElement elem = node.toElement();
            if(elem.tagName() == "emit")
            {
                if(not elem.hasAttribute("file"))
                {
                    std::cerr << filename <<
                    " : Invalid emission database : no file specified."
                    << std::endl;
                }
                if(!mEmit.load(elem.attribute("file","undef").toStdString()))
                {
                    std::cerr << filename <<
                    " : Unable to load the emit database." << std::endl;
                }
            }
            else if(elem.tagName() == "transition")
            {
                std::istringstream stream(elem.text().toStdString());
                try
                {
                    ublas::matrix<float> table;
                    boost::archive::text_iarchive ar(stream);
                    ar & table;
                    for(int i = 0; i < mNbStates; i++)
                        for(int j = 0; j < mNbStates; j++)
                            mMatrix[i][j] = table(i,j);
                }
                catch(boost::archive::archive_exception ex)
                {
                    std::cerr << filename <<
                    " : Invalid transition table : " << ex.what() <<
                    std::endl;
                }
            }
            node = node.nextSibling();
        }
    }
    else
    {
        std::cerr << filename <<
            " : Invalid document : expected <model> markup." << std::endl;
        return false;
    }

    // Compute mNbFpSeen;
    mNbFpSeen = 0;
    for(int i = 0; i < mNbStates; i++)
        mNbFpSeen += mMatrix[0][i];

    print();
    return true;
}

bool HMM::save(string filename)
{
    QFile file(filename.c_str());
    if(!file.open(QFile::WriteOnly))
        std::cerr << filename << " : Unable to open the file." << std::endl;

    QDomDocument doc;

    QDomElement rootNode = doc.createElement("model");
    rootNode.setAttribute("states", mNbStates);
    
    string dbFile = "fp.db";

    QDomElement emitElem = doc.createElement("emit");
    emitElem.setAttribute("file", dbFile.c_str());
    rootNode.appendChild(emitElem);

    mEmit.save(dbFile);

    QDomElement transNode = doc.createElement("transition");
    ublas::matrix<float> table(mNbStates, mNbStates);
    for(int i = 0; i < mNbStates; i++)
        for(int j = 0; j < mNbStates; j++)
           table(i,j) = mMatrix[i][j];
    std::ostringstream stream;
    boost::archive::text_oarchive ar(stream);
    ar & table;
    QDomText textNode = doc.createTextNode(stream.str().c_str());
    transNode.appendChild(textNode);
    rootNode.appendChild(transNode);
    
    doc.appendChild(rootNode);

    // 4 is the indentation
    file.write(doc.toString(4).toAscii());
    file.close();
    
    return true;
}

// Provided for debug purposes only
void HMM::print()
{
    std::cout << "Transition matrix :\n";
    for(int i = 0; i < mNbStates; i++)
    {
        for(int j = 0; j < mNbStates; j++)
            std::cout << mMatrix[i][j] << " ";
        std::cout << std::endl;
    }
    std::cout << "Emission probabilities :\n";
    
    std::map<fingerp,vector<int> >::const_iterator it = mEmit.d_begin();
    for(; it != mEmit.d_end(); it++)
    {
        std::cout << it->first << " : ";
        vector<int> v = it->second;
        for(unsigned int i = 0; i < v.size(); i++)
            std::cout << v[i] << " ";
        std::cout << std::endl;
    }
}

void HMM::setState(int state)
{
    if(mTraining && mNbFpSeen > 0)
    {
        mMatrix[mCurrentState][state]++;
        mMatrix[mCurrentState][mCurrentState]--;
    }
    else
    {
        std::cout << "State : " << state << std::endl;
    }
    mCurrentState = state;

    if(mObserver)
        mObserver->notify();
}

int HMM::state()
{
    return mCurrentState;
}

void HMM::consumeFingerprint(fingerp fp)
{
    if(mNbStates)
    {
        if(mTraining)
        {
            vector<int> emProb = mEmit.get(fp,mNullVector);
            mEmit.set(fp, incrementCurrent(emProb));
            mMatrix[mCurrentState][mCurrentState]++;
            mNbFpSeen++;
        }

        if(not mTraining)
        {
            mObs.push_back(fp);
            infer();
        }
    }
}

void HMM::infer()
{
    while(not mObs.empty())
    {
        fingerp fp = mObs.front();
        mObs.pop_front();
        vector<int> emitProb = mEmit.get(fp, mNullVector);
        vector<float> curProbas(mNbStates, 0);
        for(int s = 0; s < mNbStates; s++)
        {
            float maxp = 0;
            for(int s0 = 0; s0 < mNbStates; s0++)
            {
                float p = 
                  (mProbas.empty() ?
                    1 :
                    mProbas[mProbas.size()-1][s0]*
                        ((float)mMatrix[s0][s]) / mNbFpSeen);
                maxp = std::max(maxp, p);
            }
            curProbas[s] = (((float)emitProb[s]) / (mNbFpSeen)) * maxp;
        }
        mProbas.push_back(curProbas);
    }

    vector<float> latestProb = mProbas[mProbas.size()-1];
    int bestS = -1;
    for(int s = 0; s < mNbStates; s++)
    {
        if(bestS == -1 || latestProb[bestS] < latestProb[s])
            bestS = s;
    }
    setState(bestS);
}

vector<int> HMM::incrementCurrent(vector<int> v)
{
    v[mCurrentState]++;
    return v;
}

void HMM::setObserver(HMMStateObserver* obs)
{
    mObserver = obs;
}

void HMM::unregisterObserver(HMMStateObserver* obs)
{
    if(mObserver == obs)
        mObserver = 0;
}

