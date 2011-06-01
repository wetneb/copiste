#ifndef INCLUDED_SOUNDANALYSERH
#define INCLUDED_SOUNDANALYSERH

//! Implementation of StreamPlayer, designed for generic feature extraction
class SoundAnalyser;

// Libraries
#include <vector>
#include <utility>
#include <string>

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

/**
 * TODO : Register ALL the features and create a tab saying what should be used
 */

class SoundAnalyser : private StreamPlayer
{
    public:
        //! Sets up a new sound analyser
        SoundAnalyser();
        //! Destructor.
        ~SoundAnalyser();
        //! Adds a new extractor
        void registerExtractor(string name, FeatureExtractor* extr);

        //! Compute the features of a given file
        bool compute(string url);
        //! Wait up to the end of the computation
        void waitComputed();

        /// Features management

        //! Return the number of different features we computed
        unsigned int nbFeatures() { return mExtr.size(); }
        //! Return the number of elements of the nth feature
        unsigned int nbElems(unsigned int n);
        //! Return the array of features for the nth sample
        float** features(unsigned int n);
        //! Return the number of sample we've been computing
        unsigned int nbSamples() { return mFeatures.size(); }
        //! Clears the features
        void clearFeatures();
        //! Get the dimension : sum of all the nbElems(i)
        unsigned int dimension() { return mDimension; }
        //! Get the name of the nth feature
        string name(unsigned int n);

        //! Handle audio chunks
        void useBuffer();
        //! End the computation
        void sequenceEnds();

        //! Mirrors

        //! Get the sampling frequency
        unsigned int samplingFrequency() { return mFrequency; }

    private:
        // To be kept
        vector<pair<string, FeatureExtractor* > > mExtr;
        vector<float**> mFeatures;

        int mDimension;

        // To be moved
        string mBasePath;
        vector<string> mFiles;
        vector<bool> mGoals;
        int mCurrentFile;
        bool mVerbose;

        // To be kept
        boost::mutex mSwitchLock;
        bool mComputed;
        libvlc_time_t mLastUpdateTime;
};

#endif
