#include "core/soundanalyser.h"

//! Sets up a new sound analyser
SoundAnalyser::SoundAnalyser() : mDimension(0),
                                mRealDimension(0),
                                mUsedExtractors(0),
                                mComputed(false)
{
    // All the registered extractors are deleted in the destructor
    setup("pipeline/spectrum.xml");

    //ZCRExtr *zcr =  new ZCRExtr(AUDIO_CHUNK_SIZE);
    //registerExtractor("ZCR", zcr, true);
    //registerExtractor("ZCR2", zcr, true);

/*
    SpectrumExtr *spec = new SpectrumExtr(AUDIO_CHUNK_SIZE);
    registerExtractor("Spectrum", spec, false); */


    /* FluxFilter *flux = new FluxFilter;
    flux->bind(spec);
    registerExtractor("Spectrum flux", flux, false);

    AverageFilter *av = new AverageFilter;
    av->bind(flux);
    registerExtractor("Av. spec. flux", av, true); */

     /*
    RangeFilter *rg1 = new RangeFilter;
    rg1->bind(spec);
    rg1->setInt("start",0);
    rg1->setInt("end", 100);
    registerExtractor("RangeFilter1", rg1);

    RangeFilter *rg2 = new RangeFilter;
    rg2->bind(spec);
    rg2->setInt("start", 100);
    rg2->setInt("end", 800);
    registerExtractor("RangeFilter2", rg2); */

    /*
    AverageFilter *av = new AverageFilter;
    av->bind(spec);
    registerExtractor("av", av); */
    // */

    // ZCR et LSTER
    /*
    ZCRExtr *zcr =  new ZCRExtr(AUDIO_CHUNK_SIZE);
    registerExtractor("ZCR", zcr, false);
    HZCRRExtr *hzcrr = new HZCRRExtr(AUDIO_CHUNK_SIZE);
    hzcrr->setZCRExtractor(zcr);
    hzcrr->setFloat("bound", 2.0);
    hzcrr->setInt("chunksNumber", 80);
    registerExtractor("HZCRR", hzcrr, false);

    STEExtr *ste = new STEExtr(AUDIO_CHUNK_SIZE);
    registerExtractor("STE", ste, false);

    LSTERExtr *lster = new LSTERExtr(AUDIO_CHUNK_SIZE);
    registerExtractor("LSTER", lster, false);
    lster->setSTEExtractor(ste);
    lster->setFloat("bound", 0.45);

    AverageFilter *averageLSTER = new AverageFilter();
    averageLSTER->bind(lster);
    averageLSTER->setInt("size", 100);
    registerExtractor("Average LSTER", averageLSTER);

    AverageFilter* average = new AverageFilter();
    average->bind(hzcrr);
    average->setInt("size", 100);
    registerExtractor("Average HZCRR", average); */

    mLastUpdateTime = 0;
}

//! Set up from an XML file
bool SoundAnalyser::setup(string filename)
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

                if(flt == 0)
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

                registerExtractor(name, extr, true);
                cout << "Registered "<<name<<" : reald : "<<realDimension()<<".\n";
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
}

//! Destructor.
SoundAnalyser::~SoundAnalyser()
{
    resetExtractors();
    clearFeatures();
}

//! Clears the features
void SoundAnalyser::clearFeatures()
{
    for(unsigned int i = 0; i < mFeatures.size(); i++)
    {
        if(mFeatures[i])
        {
            for(unsigned int j = 0; j < mExtr.size(); ++j)
            {
                if(mFeatures[i][j])
                    delete mFeatures[i][j];
            }
            delete mFeatures[i];
        }
    }
    mFeatures.clear();
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
    if(mLastUpdateTime != 0)
    {
        cout << "\e[F\e[KDone." << endl;
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
        if(mLastUpdateTime < playingTime() - 500)
        {
            if(mLastUpdateTime != 0)
                cout << "\e[F" << flush; // Magic sequence which flushes the last written line (not so magic, see ANSI spec)
            cout << "Computing... " << 100 * playingTime() / totalTime() << "%" << endl;
            mLastUpdateTime = playingTime();
        }
        float **featureArray = new float*[mExtr.size()]; // deleted in clearFeatures()
        for(unsigned int i = 0; i < mExtr.size(); ++i)
            featureArray[i] = new float[mExtr[i].second->size()]; // deleted in clearFeatures()

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
    boost::posix_time::seconds waitTime(1);

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
float** SoundAnalyser::features(unsigned int n)
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
