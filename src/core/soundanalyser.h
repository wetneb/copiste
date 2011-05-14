#ifndef INCLUDED_SOUNDANALYSERH
#define INCLUDED_SOUNDANALYSERH

//! Implementation of StreamPlayer, designed for feature extraction
class SoundAnalyser;

// Libraries
#include <vector>
#include <utility>
#include <string>
#include <boost/filesystem.hpp>

// Stream handling
#include "core/streamplayer.h"
#include "features/extractor.h"

// Features
#include "features/spectrum.h"
#include "features/zcr.h"
#include "features/hzcrr.h"
#include "features/ste.h"
#include "features/lster.h"

// Storage
#include "algo/corpus.h"

using namespace std;

class SoundAnalyser : public StreamPlayer
{
    public:
        //! Sets up a new sound analyser
        SoundAnalyser();
        //! Destructor.
        ~SoundAnalyser();
        //! Adds a new extractor
        void registerExtractor(string name, FeatureExtractor* extr);

        //! Set up from an XML file
        bool setup(string fileName);
        //! Write down the results
        bool write(Corpus* corpus);

        //! Compute the features
        bool compute();
        //! Wait up to the end of the computation
        void waitComputed();

        //! Handle audio chunks
        void useBuffer();
        //! Switch to the next file
        void sequenceEnds();

        //! Sets verbosity
        void setVerbose(bool verbose) { mVerbose = verbose; }

    private:
        string mBasePath;
        vector<pair<string, FeatureExtractor* > > mExtr;
        vector<string> mFiles;
        vector<bool> mGoals;
        vector<float*> mFeatures;
        int mDimension;

        boost::mutex mSwitchLock;
        long int mNbChunks;
        int mCurrentFile;
        bool mComputed;
        bool mVerbose;
};

#endif
