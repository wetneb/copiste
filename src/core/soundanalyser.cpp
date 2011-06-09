#include "core/soundanalyser.h"

//! Sets up a new sound analyser
SoundAnalyser::SoundAnalyser() : mDimension(0),
                                mRealDimension(0),
                                mUsedExtractors(0),
                                mBasePath("."),
                                mCurrentFile(-1),
                                mVerbose(false),
                                mComputed(false)
{
    // All the registered extractors are deleted in the destructor
    //! TODO : dynamic extractor handling

    //registerExtractor("Spectrum", new SpectrumExtr(AUDIO_CHUNK_SIZE));
    ZCRExtr *zcr =  new ZCRExtr(AUDIO_CHUNK_SIZE);
    registerExtractor("ZCR", zcr, false);
    HZCRRExtr *hzcrr = new HZCRRExtr(AUDIO_CHUNK_SIZE);
    hzcrr->setZCRExtractor(zcr);
    hzcrr->setFloat("bound", 2.0);
    hzcrr->setInt("chunksNumber", 80);
    registerExtractor("HZCRR", hzcrr, false);

/*
    HZCRRExtr *hzcrr1 = new HZCRRExtr(AUDIO_CHUNK_SIZE);
    hzcrr1->setInt("chunksNumber", 80);
    hzcrr1->setZCRExtractor(zcr);
    hzcrr1->setFloat("bound", 2.0);
    registerExtractor("HZCRR-1", hzcrr1);

    HZCRRExtr *hzcrr2 = new HZCRRExtr(AUDIO_CHUNK_SIZE);
    hzcrr2->setInt("chunksNumber", 80);
    hzcrr1->setZCRExtractor(zcr);
    hzcrr2->setFloat("bound", 2.2);
    registerExtractor("HZCRR-2", hzcrr2); */


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
    registerExtractor("Average HZCRR", average);

    mLastUpdateTime = 0;
}

//! Destructor.
SoundAnalyser::~SoundAnalyser()
{
    for(unsigned int i = 0; i < mExtr.size(); i++)
        delete mExtr[i].second;
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

/*
        vector<pair<string, FeatureExtractor* > > mFeatures;
        vector<string> mFiles;
        vector<float*> mFeatures;
*/
