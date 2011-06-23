#include "core/corpusbuilder.h"

//! Sets up a new corpus builder
CorpusBuilder::CorpusBuilder() : mBasePath("."),
                                mCurrentFile(-1),
                                mVerbose(false),
                                mElemLength(0),
                                mCompOffset(0)
{
    ;
}

bool isPrefix(string pre, string str)
{
    unsigned int index = 0;
    while(index < pre.size() && index < str.size() && str[index] == pre[index])
        index++;
    return (index == pre.size());
}

//! Set up from an XML file
bool CorpusBuilder::setup(string fileName)
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
                rules.push_back(make_pair(elem.attribute("prefix", "").toStdString(), (elem.attribute("goal", "1").toInt() == 1)));

            node = node.nextSibling();
        }
    }
    else
    {
        cout << "Invalid document : expecting <target> markup." << endl;
        return false;
    }

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
                    mNbElems.push_back(0);
                }
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
void CorpusBuilder::compute()
{
    if(mFiles.size())
    {
        mCurrentFile = 0;

        // Clean the previous results
        for(unsigned int i = 0; i < mResults.size(); i++)
            if(mResults[i])
                delete mResults[i];
        mResults.clear();

        // Loop through files
        while((unsigned int)mCurrentFile < mFiles.size())
        {
            // Set up the media player
            if(mVerbose)
            {
                string filename = boost::filesystem::path(mFiles[mCurrentFile]).stem().string();
                cout << "["<<mCurrentFile+1<<"/"<<mFiles.size()<<"] : "<<filename<<endl;
            }

            // Do the computation
            ((SoundAnalyser*)this)->compute(mFiles[mCurrentFile]);
            waitComputed();

            // Save the features
            const int startingPoint = mCompOffset * samplingFrequency() / AUDIO_CHUNK_SIZE;
            const int length = ((mElemLength == 0) ? (nbSamples() - startingPoint - 1) : (mElemLength * samplingFrequency() / AUDIO_CHUNK_SIZE));

            float *mCurrentResults = 0;
            for(unsigned int k = startingPoint; k < nbSamples(); k++) // Loop through samples
            {
                if(((k - startingPoint) % length) == 0 || k == nbSamples() - 1)
                {
                    // Save the old one (if any)
                    if(mCurrentResults != 0)
                    {
                        for(unsigned int i = 0; i < realDimension(); ++i)
                            mCurrentResults[i] /= length;
                    }

                    // Create a new one (if any)
                    if(nbSamples() - k >= (unsigned int)length)
                    {
                        mNbElems[mCurrentFile]++;
                        mCurrentResults = new float[realDimension()];
                        mResults.push_back(mCurrentResults);
                        for(unsigned int i = 0; i < realDimension(); ++i)
                            mCurrentResults[i] = 0;
                    }
                }

                int saved = 0;
                for(unsigned int i = 0; i < nbFeatures(); i++) // Loop through features
                {
                    if(isUsed(i))
                    {
                        for(unsigned int j = 0; j < nbElems(i); j++) // Loop through the elements of the feature
                            mCurrentResults[saved + j] += features(k)[i][j];
                        saved += nbElems(i);
                    }
                }
            }

            clearFeatures();
            cleanExtractors();

            mCurrentFile++;
        }
    }
}

//! Write down the results
bool CorpusBuilder::write(Corpus *corpus)
{
    if(corpus->dimension() != (unsigned int)realDimension())
        corpus->erase(realDimension());

    int saved = 0;
    for(unsigned int i = 0; i < mFiles.size(); ++i)
    {
        for(int k = 0; k < mNbElems[i] && (unsigned int)saved < mResults.size(); ++k)
        {
            // Declare a new sample
            float* sample = new float[realDimension()+1]; // deleted by corpus->erase()
            sample[0] = 0; // Default value

            // Bind the file to a class
            if(i < mFiles.size())
                sample[0] = (mGoals[i] ? 1 : (-1));

            // Add the features
            for(unsigned int j = 0; j < realDimension(); j++)
                sample[j+1] = mResults[saved][j];

            // Add it to the corpus
            corpus->addElem(sample);

            saved++;
        }
    }
    //mSwitchLock.unlock();
    return true;
}

