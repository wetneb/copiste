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
#if BOOST_VERSION <= 104200
		string filename = iter->path().filename();
#else
                string filename = iter->path().filename().string();
#endif

                // Search the filename in the prefix table
                unsigned int prefixId = 0;
                for(; prefixId < rules.size() && !isPrefix(rules[prefixId].first, filename); prefixId++);


                if(prefixId < rules.size())
                {
#if BOOST_VERSION <= 104200
		    mFiles.push_back(string("file://" + boost::filesystem::system_complete(iter->path()).string()));
#else
                    mFiles.push_back(string("file://" + boost::filesystem::absolute(iter->path()).string()));
#endif
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
            delete [] mResults[i];
        mResults.clear();

        // Loop through files
        while((unsigned int)mCurrentFile < mFiles.size())
        {
            // Set up the media player
#if BOOST_VERSION <= 104200
	    string filename = boost::filesystem::path(mFiles[mCurrentFile]).stem();
#else
            string filename = boost::filesystem::path(mFiles[mCurrentFile]).stem().string();
#endif
            if(mVerbose)
                cout << "["<<mCurrentFile+1<<"/"<<mFiles.size()<<"] : "<<filename<<endl;

            // Save the filename (so that the corpus can be labelled)
            mSampleNames.push_back(filename);

            // Do the computation
            ((SoundAnalyser*)this)->compute(mFiles[mCurrentFile]);
            waitComputed();

            // Save the features
            const int startingPoint = mCompOffset * samplingFrequency() / getChunkSize();
            const int length = ((mElemLength == 0) ?
                                     (nbSamples() - startingPoint - 1)
                                   : (mElemLength * samplingFrequency() / getChunkSize()));

            double *mCurrentResults = 0;
            for(unsigned int k = startingPoint; k < nbSamples() - startingPoint; k++) // Loop through samples
            {
                if(((k - startingPoint) % length) == 0 // If we reached the end of a sequence
                                    || k == nbSamples() - 1) // or if we reached the end of the file
                {
                    // Save the old one (if any)
                    if(mCurrentResults != 0)
                    {
                        // Just compute the average of the features
                        for(unsigned int i = 0; i < realDimension(); ++i)
                            mCurrentResults[i] /= length;
                    }

                    // Create a new one (if any)
                    if(nbSamples() - k >= (unsigned int)length) // new segment
                    {
                        mNbElems[mCurrentFile]++;

                        // Create a new array to store the features
                        mCurrentResults = new double[realDimension()];
                        mResults.push_back(mCurrentResults);

                        // Init the array
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
            double* sample = new double[realDimension()+1]; // deleted by corpus->erase()
            sample[0] = 0; // Default value

            // Bind the file to a class
            if(i < mFiles.size())
                sample[0] = (mGoals[i] ? 1 : 0);

            // Add the features
            for(unsigned int j = 0; j < realDimension(); j++)
                sample[j+1] = mResults[saved][j];

            // Add it to the corpus
            corpus->addElem(sample, mSampleNames[i]);

            saved++;
        }
    }
    //mSwitchLock.unlock();
    return true;
}

CorpusBuilder::~CorpusBuilder()
{
    for(unsigned int i = 0; i < mResults.size(); i++)
        delete [] mResults[i];
    mResults.clear();
}

