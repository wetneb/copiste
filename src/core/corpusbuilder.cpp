#include "core/corpusbuilder.h"

//! Sets up a new corpus builder
CorpusBuilder::CorpusBuilder() : mBasePath("."), mCurrentFile(-1), mVerbose(false)
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
void CorpusBuilder::compute()
{
    if(mFiles.size())
    {
        mCurrentFile = 0;
        mResults.clear(); // TODO : This is not clean : we have to do the deletes

        while((unsigned int)mCurrentFile < mFiles.size())
        {
            mResults.push_back(new float[realDimension()]);
            for(unsigned int i = 0; i < realDimension(); ++i)
                mResults[mCurrentFile][i] = 0;

            // Set up the media player
            if(mVerbose)
            {
                string filename = boost::filesystem::path(mFiles[mCurrentFile]).filename().string();
                std::cout << "["<<mCurrentFile + 1<<"/"<<mFiles.size()<<"] : "<<filename<< std::endl;
            }

            ((SoundAnalyser*)this)->compute(mFiles[mCurrentFile]);
            waitComputed();

            // Save the features
            int saved = 0;
            for(unsigned int i = 0; i < nbFeatures(); i++)
            {
                if(isUsed(i))
                {
                    for(unsigned int j = 0; j < nbElems(i); j++)
                    {
                        mResults[mCurrentFile][saved + j] = 0;
                        // Compute the average
                        for(unsigned int k = 0; k < nbSamples(); k++)
                            mResults[mCurrentFile][saved + j] += features(k)[i][j];
                        mResults[mCurrentFile][saved + j] /= nbSamples();

                        cout << name(i) << "["<<j+1<<"] :" << mResults[mCurrentFile][saved + j] << endl;
                    }
                    saved += nbElems(i);
                }
            }

            mCurrentFile++;
        }
    }
}

//! Write down the results
bool CorpusBuilder::write(Corpus *corpus)
{
    if(corpus->dimension() != (unsigned int)realDimension())
        corpus->erase(realDimension());

    for(unsigned int i = 0; i < mResults.size(); ++i)
    {
        // Declare a new sample
        float* sample = new float[realDimension()+1]; // deleted by corpus->erase()
        sample[0] = 0; // Default value

        // Bind the file to a class (to be changed)
        if(i < mFiles.size())
        {
            boost::filesystem::path pt(mFiles[i]);
            sample[0] = (mGoals[i] ? 1 : (-1));
        }

        // Add the features
        for(unsigned int j = 0; j < realDimension(); j++)
            sample[j+1] = mResults[i][j];

        // Add it to the corpus
        corpus->addElem(sample);
    }
    //mSwitchLock.unlock();
    return true;
}

