#include "core/soundanalyser.h"

//! Sets up a new sound analyser
SoundAnalyser::SoundAnalyser() : mBasePath("."), mDimension(0), mNbSamples(0), mCurrentFile(-1), mVerbose(false)
{
    //registerExtractor("Spectrum", new SpectrumExtr(AUDIO_CHUNK_SIZE));
    registerExtractor("ZCR", new ZCRExtr(AUDIO_CHUNK_SIZE));
    registerExtractor("HZCRR", new HZCRRExtr(AUDIO_CHUNK_SIZE));
    registerExtractor("STE", new STEExtr(AUDIO_CHUNK_SIZE));
    registerExtractor("LSTER", new LSTERExtr(AUDIO_CHUNK_SIZE));
}

//! Destructor.
SoundAnalyser::~SoundAnalyser()
{
    for(unsigned int i = 0; i < mExtr.size(); i++)
        delete mExtr[i].second;
}

//! Set up from an XML file
bool SoundAnalyser::setup(string fileName)
{
    // Here some XML loading code

    return true;
}

//! Compute the features
bool SoundAnalyser::compute()
{
    // Compute the number of coordinates we have to store
    mDimension = 0;
    for(unsigned int i = 0; i < mExtr.size(); i++)
        mDimension += (mExtr[i]).second->size();
    cout << "Dimension : "<< mDimension << endl;

    // Find the audio files
    if(boost::filesystem::is_directory(mBasePath))
    {
        /// Longue vie à boost::filesystem !
        for (boost::filesystem::directory_iterator iter(mBasePath); iter != boost::filesystem::directory_iterator(); ++iter)
        {
            bool validFile = false;

            if(boost::filesystem::is_regular_file(iter->status()))
            {
                string filename = iter->path().filename().string();
                if(filename[0] == 'A' || filename[0] =='B')
                    validFile = true;
            }

            if(validFile)
                mFiles.push_back(string("file://" + boost::filesystem::absolute(iter->path()).string()));
            else if(mVerbose)
                std::cout << "Ignoring "<<iter->path().filename() << std::endl;
        }
    }
    else
    {
        std::cerr << "Unable to read audio corpus : wrong base path." << std::endl;
        return false;
    }

    if(mFiles.size())
    {
        // Clean up the previous data
        for(unsigned int i = 0; i < mFeatures.size(); ++i)
            if(mFeatures[i])
                delete mFeatures[i];

        mFeatures.resize(mFiles.size(), 0);

        sequenceEnds();
    }
    return true;
}

void SoundAnalyser::sequenceEnds()
{
    mSwitchLock.lock();

    // Store the old file
    if(mCurrentFile >= 0)
    {
        cout << "Results :"<< endl;
        for(int i = 0; i < mDimension; i++)
        {
            mFeatures[mCurrentFile][i] /= mNbSamples;
            cout << mExtr[i].first << " :\t" << mFeatures[mCurrentFile][i] << endl;
        }
    }

    mCurrentFile++;

    // Start a new file
    if(mCurrentFile < (int)mFiles.size())
    {
        mNbSamples = 0;

        mFeatures[mCurrentFile] = new float[mDimension];
        for(int i = 0; i < mDimension; ++i)
            mFeatures[mCurrentFile][i] = 0;

        // Set up the media player
        if(mVerbose)
            std::cout << "["<<mCurrentFile + 1<<"/"<<mFiles.size()<<"] : "<<mFiles[mCurrentFile]<< std::endl;
        setUrl(mFiles[mCurrentFile]);
        play();
    }

    mSwitchLock.unlock();
}

void SoundAnalyser::useBuffer()
{
    // For each feature
    int featuresSize = 0;
    for(unsigned int i = 0; i < mExtr.size(); ++i)
    {
        // Compute
        FeatureExtractor *extr = mExtr[i].second;
        extr->extract(mBuffer, AUDIO_CHUNK_SIZE);
        for(int j = 0; j < extr->size(); j++)
            mFeatures[mCurrentFile][featuresSize + j] = extr->value(j);

        featuresSize += extr->size();
    }

    mNbSamples += AUDIO_CHUNK_SIZE;
}

//! Write down the results
bool SoundAnalyser::write(string fileName)
{
    // Some XML writing code

    return true;
}

//! Adds a new extractor
void SoundAnalyser::registerExtractor(string name, FeatureExtractor* extr)
{
    if(extr)
        mExtr.push_back(make_pair(name, extr));
}

void SoundAnalyser::waitComputed()
{
    boost::posix_time::seconds waitTime(1);
    bool computed = false;
    while(!computed)
    {
        boost::this_thread::sleep(waitTime);
        mSwitchLock.lock();
        computed = (mCurrentFile == -1 || mCurrentFile >= (int)mFiles.size());
        mSwitchLock.unlock();
    }
}

/*
        vector<pair<string, FeatureExtractor* > > mFeatures;
        vector<string> mFiles;
        vector<float*> mFeatures;
*/
