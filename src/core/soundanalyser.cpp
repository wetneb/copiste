#include "core/soundanalyser.h"

//! Sets up a new sound analyser
SoundAnalyser::SoundAnalyser() : mBasePath("."), mDimension(0), mNbChunks(0), mCurrentFile(-1), mComputed(false), mVerbose(false)
{
    //registerExtractor("Spectrum", new SpectrumExtr(AUDIO_CHUNK_SIZE));
    //registerExtractor("ZCR", new ZCRExtr(AUDIO_CHUNK_SIZE));
    registerExtractor("HZCRR", new HZCRRExtr(AUDIO_CHUNK_SIZE));
    //registerExtractor("STE", new STEExtr(AUDIO_CHUNK_SIZE));
    registerExtractor("LSTER", new LSTERExtr(AUDIO_CHUNK_SIZE));
}

//! Destructor.
SoundAnalyser::~SoundAnalyser()
{
    for(unsigned int i = 0; i < mExtr.size(); i++)
        delete mExtr[i].second;
    //if(mSwitchLock.try_lock())
     //   mSwitchLock.unlock();
}

bool isPrefix(string pre, string str)
{
    unsigned int index = 0;
    while(index < pre.size() && index < str.size() && str[index] == pre[index])
        index++;
    return (index == pre.size());
}

//! Set up from an XML file
bool SoundAnalyser::setup(string fileName)
{
    // Load the discrimination rules
    QDomDocument doc;
    vector<pair<string, bool> > rules;

    // Load the file and parse it
    QFile file(fileName.c_str());
    if(!file.open(QIODevice::ReadOnly))
    {
        cerr << "Unable to open file : " << fileName << endl;
        return false;
    }
    doc.setContent(&file);
    file.close();

    // Read the contents
    QDomNode node = doc.documentElement();

    if(node.toElement().tagName() == "target")
    {
        mBasePath = node.toElement().attribute("corpus", "").toStdString();

        node = node.firstChild();
        while(!node.isNull())
        {
            QDomElement elem = node.toElement();

            if(elem.tagName() == "class" && elem.attribute("prefix", "") != "")
                rules.push_back(make_pair(elem.attribute("prefix", "").toStdString(), (elem.attribute("goal", "1").toInt() != 0)));

            node = node.nextSibling();
        }
    }
    else
    {
        cout << "Invalid document : expecting <target> markup." << endl;
        return false;
    }

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
            if(boost::filesystem::is_regular_file(iter->status()))
            {
                string filename = iter->path().filename().string();

                // Search the filename in the prefix table
                unsigned int prefixId = 0;
                for(; prefixId < rules.size() && !isPrefix(rules[prefixId].first, filename); prefixId++);


                if(prefixId < rules.size())
                {
                    mFiles.push_back(string("file://" + boost::filesystem::absolute(iter->path()).string()));
                    mGoals.push_back((int)(rules[prefixId].second));
                }
                else
                    std::cout << "Ignoring "<< filename << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Unable to read audio corpus : wrong base path." << std::endl;
        return false;
    }

    return true;
}

//! Compute the features
bool SoundAnalyser::compute()
{
    if(mFiles.size())
    {
        // Clean up the previous data
        for(unsigned int i = 0; i < mFeatures.size(); ++i)
            if(mFeatures[i])
                delete mFeatures[i];

        mFeatures.resize(mFiles.size(), 0);

        sequenceEnds();
    }
    else
        mComputed = true;

    return true;
}

void SoundAnalyser::sequenceEnds()
{
    mSwitchLock.lock();

    // Store the old file
    if(mCurrentFile >= 0)
    {
        int printed = 0;
        for(unsigned int i = 0; i < mExtr.size(); i++)
        {
            for(int j = 0; j < mExtr[i].second->size(); j++)
            {
                mFeatures[mCurrentFile][printed + j] /= mNbChunks;
                cout << mExtr[i].first << "["<<j+1<<"] :" << mFeatures[mCurrentFile][printed + j] << endl;
            }
            printed += mExtr[i].second->size();
        }
    }

    mCurrentFile++;
    // Start a new file
    if(mCurrentFile < (int)mFiles.size())
    {
        mNbChunks = 0;

        mFeatures[mCurrentFile] = new float[mDimension];
        for(int i = 0; i < mDimension; ++i)
            mFeatures[mCurrentFile][i] = 0;

        // Set up the media player
        if(mVerbose)
        {
            string filename = boost::filesystem::path(mFiles[mCurrentFile]).filename().string();
            std::cout << "["<<mCurrentFile + 1<<"/"<<mFiles.size()<<"] : "<<filename<< std::endl;
        }

        setUrl(mFiles[mCurrentFile]);
        play();
    }
    else mComputed = true;

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

    mNbChunks++;
}

//! Write down the results
bool SoundAnalyser::write(Corpus *corpus)
{
    //waitComputed();

    if(corpus->dimension() != (unsigned int)mDimension)
        corpus->erase(mDimension);

    for(unsigned int i = 0; i < mFeatures.size(); ++i)
    {
        // Declare a new sample
        float* sample = new float[mDimension+1];
        sample[0] = 0; // Default value

        // Bind the file to a class (to be changed)
        if(i < mFiles.size())
        {
            boost::filesystem::path pt(mFiles[i]);
            sample[0] = mGoals[i];
        }

        // Add the features
        for(int j = 0; j < mDimension; j++)
            sample[j+1] = mFeatures[i][j];

        // Add it to the corpus
        corpus->addElem(sample);
    }
    //mSwitchLock.unlock();
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

    mSwitchLock.lock();
    while(!mComputed)
    {
        mSwitchLock.unlock();
        boost::this_thread::sleep(waitTime);
    }
    cout << "Returning" << endl;
}

/*
        vector<pair<string, FeatureExtractor* > > mFeatures;
        vector<string> mFiles;
        vector<float*> mFeatures;
*/
