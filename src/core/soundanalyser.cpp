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


#include "core/soundanalyser.h"

//! Sets up a new sound analyser
SoundAnalyser::SoundAnalyser(bool live) : StreamPlayer(live),
                                mDimension(0),
                                mRealDimension(0),
                                mUsedExtractors(0),
                                mComputed(false)
{
    mLastUpdateTime = 0;
}

//! Set up from an XML file
bool SoundAnalyser::setupPipeline(string filename)
{
    resetExtractors();

    // Load the file and parse it
    QDomDocument doc;
    QFile file(filename.c_str());
    if(!file.open(QIODevice::ReadOnly))
    {
        cerr << "Unable to open file : " << filename << endl;
        return false;
    }
    doc.setContent(&file);
    file.close();

    // Read the contents
    QDomNode node = doc.documentElement();

    if(node.toElement().tagName() == "pipeline")
    {
        node = node.firstChild();

        while(!node.isNull())
        {
            QDomElement elem = node.toElement();
            FeatureExtractor *extr = 0;

            string name = elem.attribute("name", "unknown").toStdString();
            string type = elem.attribute("type", "unknown").toStdString();
            if(elem.tagName() == "feature" && type != "unknown")
            {
                if(type == "ZCR")
                    extr = new ZCRExtr(AUDIO_CHUNK_SIZE);
                else if(type == "STE")
                    extr = new STEExtr(AUDIO_CHUNK_SIZE);
                else if(type == "Spectrum")
                    extr = new SpectrumExtr(AUDIO_CHUNK_SIZE);
                else if(type == "HZCRR")
                    extr = new HZCRRExtr(AUDIO_CHUNK_SIZE);
                else if(type == "LSTER")
                    extr = new LSTERExtr(AUDIO_CHUNK_SIZE);
            }
            else if(elem.tagName() == "filter" && type != "unknown")
            {
                Filter *flt = 0;

                if(type == "Average")
                    flt = new AverageFilter;
                else if(type == "Range")
                    flt = new RangeFilter;
                else if(type == "Flux")
                    flt = new FluxFilter;
                else if(type == "Centroid")
                    flt = new CentroidFilter;
                else if(type == "HighLowRatio")
                    flt = new HighLowRatioFilter;
		else if(type == "Memory")
	            flt = new MemoryFilter;

                if(flt != 0)
                {
                    QDomElement child = node.firstChildElement();
                    if(!child.isNull() && child.tagName() == "ref")
                    {
                        FeatureExtractor *parent = getExtractor(child.attribute("name", "_").toStdString());
                        if(parent != 0)
                            flt->bind(parent);
                        else
                        {
                            cout << filename << " : Invalid reference : " << child.attribute("name", "_").toStdString() << endl;
                            delete flt;
                            flt = 0;
                        }
                    }
                    else
                        cout << filename << " : Missing <ref> for "<<name <<".\n";
                }
                else
                {
                    cout << filename << " : Unable to load filter "<<name<<" of type "<<type<<"."<<endl;
                }

                extr = flt;
            }
            else if(elem.tagName() == "filter" || elem.tagName() == "feature")
                cout << filename << " : Warning : type of "<<name<<" is missing, it will be ignored." << endl;

            if(extr != 0)
            {
                QDomNamedNodeMap attributes = elem.attributes();

                for(int i = 0; i < attributes.size(); i++)
                {
                    QDomAttr attr = attributes.item(i).toAttr();
                    if(attr.name() != "type" && attr.name() != "name")
                    {
                        extr->setInt(attr.name().toStdString(), attr.value().toInt());
                        extr->setFloat(attr.name().toStdString(), attr.value().toFloat());
                    }
                }

                registerExtractor(name, extr, elem.attribute("used", "yes") == "yes");
            }

            node = node.nextSibling();
        }
    }
    else
    {
        cout << filename << " : Invalid root markup : has to be <pipeline>." << endl;
        return false;
    }

    return true;
}

//! Unregisters all the extractors
void SoundAnalyser::resetExtractors()
{
    for(unsigned int i = 0; i < mExtr.size(); i++)
        delete mExtr[i].second;
    mExtr.clear();
    mUsed.clear();
}

//! Destructor.
SoundAnalyser::~SoundAnalyser()
{
    clearFeatures();
    resetExtractors();
}

//! Clears the features
void SoundAnalyser::clearFeatures()
{
    for(unsigned int i = 0; i < mFeatures.size(); i++)
    {
        if(mFeatures[i])
        {
            for(unsigned int j = 0; j < mExtr.size(); j++)
            {
                delete [] mFeatures[i][j];
            }
            delete [] mFeatures[i];
        }
    }
    mFeatures.clear();
}

//! Removes the old features so that the newest remain
void SoundAnalyser::cleanOldFeatures(unsigned int newestCount)
{
    if(nbSamples() > newestCount)
    {
        for(unsigned int i = 0; i < mFeatures.size() - newestCount; i++)
        {
            for(unsigned int j = 0; j < mExtr.size(); j++)
                delete [] mFeatures[i][j];
        }
        mFeatures.erase(mFeatures.begin(), mFeatures.end() - newestCount);
    }
}

//! Clears the data in the extractors
void SoundAnalyser::cleanExtractors()
{
    for(unsigned int i = 0; i < mExtr.size(); i++)
        mExtr[i].second->clear();
}

//! Compute the features of a given file
bool SoundAnalyser::compute(string url)
{
    clearFeatures();
    setUrl(url);
    play();

    return true;
}

void SoundAnalyser::sequenceEnds()
{
    if(mVerbose && mLastUpdateTime != 0)
    {
        cout << "\e[F\e[K" << flush;
         mLastUpdateTime = 0;
    }

    //mSwitchLock.lock();

    mComputed = true;

    //mSwitchLock.unlock();
}

void SoundAnalyser::useBuffer()
{
    if(mExtr.size())
    {
        if(mVerbose && mLastUpdateTime < playingTime() - 500)
        {
            if(mLastUpdateTime != 0)
                cout << "\e[F" << flush; // Magic sequence which flushes the last written line (not so magic, see ANSI spec)
            if(!isLive())
                cout << "Computing... " << 100 * playingTime() / totalTime() << "%" << endl;
            else cout << "Playing... " << playingTime() / 1000 << " seconds"<< endl;
            mLastUpdateTime = playingTime();
        }
        double **featureArray = new double*[mExtr.size()]; // deleted in clearFeatures()
        for(unsigned int i = 0; i < mExtr.size(); ++i)
            featureArray[i] = new double[mExtr[i].second->size()]; // deleted in clearFeatures()

        // For each feature
        for(unsigned int i = 0; i < mExtr.size(); ++i)
        {
            // Compute
            FeatureExtractor *extr = mExtr[i].second;
            extr->extract(mBuffer, AUDIO_CHUNK_SIZE);
            for(int j = 0; j < extr->size(); j++)
                featureArray[i][j] = extr->value(j);
        }
        mFeatures.push_back(featureArray);

        useFeatures();
    }
}

//! Adds a new extractor
void SoundAnalyser::registerExtractor(string name, FeatureExtractor* extr, bool used)
{
    if(extr)
    {
        mExtr.push_back(make_pair(name, extr));
        mUsed.push_back(used);

        // Update the dimension
        mDimension += extr->size();
        if(used)
            mRealDimension += extr->size();

        if(used)
            mUsedExtractors++;

    }
}

void SoundAnalyser::waitComputed()
{
    boost::posix_time::millisec waitTime(250);

    //mSwitchLock.lock();
    while(!mComputed)
    {
        //mSwitchLock.unlock();
        boost::this_thread::sleep(waitTime);

        //mSwitchLock.lock();
    }
    //mSwitchLock.unlock();
    mComputed = false;
}

//! Is this feature used for detection ?
bool SoundAnalyser::isUsed(unsigned int index)
{
    if(index < mUsed.size())
        return mUsed[index];
    return false;
}

//! Return the number of elements of the nth feature
unsigned int SoundAnalyser::nbElems(unsigned int n)
{
    if(n < mExtr.size())
        return mExtr[n].second->size();
    return 0;
}

//! Return the array of features at a given time
double** SoundAnalyser::features(unsigned int n)
{
    if(n < mFeatures.size())
        return mFeatures[n];
    return 0;
}

string SoundAnalyser::name(unsigned int n)
{
    if(n < mExtr.size())
        return mExtr[n].first;
    return "Empty";
}

FeatureExtractor* SoundAnalyser::getExtractor(string name)
{
    FeatureExtractor *res = 0;
    for(unsigned int i = 0; i < mExtr.size() && res == 0; i++)
        if(mExtr[i].first == name)
            res = mExtr[i].second;
    return res;
}

//! Get the id of the feature named so (-1 if not found)
int SoundAnalyser::getFeatureByName(string name)
{
    int res = -1, i = 0;
    while(res == -1 && i < (int)mExtr.size())
    {
        if(mExtr[i].first == name)
            res = i;
        i++;
    }
    return res;
}
