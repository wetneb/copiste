#include "core/soundanalyser.h"

//! Sets up a new sound analyser
SoundAnalyser::SoundAnalyser() : mDimension(0), mBasePath("."), mCurrentFile(-1), mVerbose(false), mComputed(false)
{
    //registerExtractor("Spectrum", new SpectrumExtr(AUDIO_CHUNK_SIZE));
    registerExtractor("ZCR", new ZCRExtr(AUDIO_CHUNK_SIZE));
    //registerExtractor("HZCRR", new HZCRRExtr(AUDIO_CHUNK_SIZE));
    registerExtractor("STE", new STEExtr(AUDIO_CHUNK_SIZE));
    //registerExtractor("LSTER", new LSTERExtr(AUDIO_CHUNK_SIZE));
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
    mSwitchLock.lock();

    mComputed = true;

    mSwitchLock.unlock();
}

void SoundAnalyser::useBuffer()
{
    if(mExtr.size())
    {
        float **featureArray = new float*[mExtr.size()];
        for(unsigned int i = 0; i < mExtr.size(); ++i)
            featureArray[i] = new float[mExtr[i].second->size()];

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
void SoundAnalyser::registerExtractor(string name, FeatureExtractor* extr)
{
    if(extr)
    {
        mExtr.push_back(make_pair(name, extr));

        // Update the dimension
        mDimension += extr->size();
    }
}

void SoundAnalyser::waitComputed()
{
    boost::posix_time::seconds waitTime(1);

    mSwitchLock.lock();
    while(!mComputed)
    {
        mSwitchLock.unlock();
        boost::this_thread::sleep(waitTime);

        mSwitchLock.lock();
    }
    mSwitchLock.unlock();
    mComputed = false;
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
