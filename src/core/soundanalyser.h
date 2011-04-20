#ifndef INCLUDED_SOUNDANALYSERH
#define INCLUDED_SOUNDANALYSERH

//! Implementation of StreamPlayer, designed for feature extraction
class SoundAnalyser;

#include <vector>
#include <utility>
#include <string>
#include <boost/filesystem.hpp>

#include "core/streamplayer.h"
#include "features/extractor.h"

// Features
#include "features/spectrum.h"
#include "features/zcr.h"
#include "features/hzcrr.h"
#include "features/ste.h"
#include "features/lster.h"

using namespace std;

class SoundAnalyser : public StreamPlayer
{
    public:
        //! Sets up a new sound analyser
        SoundAnalyser();

        //! Destructor.
        ~SoundAnalyser();

        //! Set up from an XML file
        bool setup(string fileName);
        //! Set the corpus directory
        void setBasePath(string basePath) { mBasePath = basePath; }

        //! Compute the features
        bool compute();
        //! Wait up to the end of the computation
        void waitComputed();

        //! Handle audio chunks
        void useBuffer();
        //! Switch to the next file
        void sequenceEnds();

        //! Write down the results
        bool write(string fileName);

        //! Adds a new extractor
        void registerExtractor(string name, FeatureExtractor* extr);

        //! Sets verbosity
        void setVerbose(bool verbose) { mVerbose = verbose; }

    private:
        string mBasePath;
        vector<pair<string, FeatureExtractor* > > mExtr;
        vector<string> mFiles;
        vector<float*> mFeatures;
        int mDimension;

        boost::mutex mSwitchLock;
        long int mNbSamples;
        int mCurrentFile;
        bool mVerbose;
};

#endif
