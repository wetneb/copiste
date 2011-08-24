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

// Filters
#include "filters/average.h"
#include "filters/flux.h"
#include "filters/range.h"
#include "filters/highlowratio.h"
#include "filters/centroid.h"

using namespace std;


/**
 * \class SoundAnalyser
 * \brief Main class that sets up and runs a pipeline
 */
class SoundAnalyser : private StreamPlayer
{
    public:
        //! Sets up a new sound analyser
        SoundAnalyser(bool live = false);
        //! Destructor.
        ~SoundAnalyser();
        //! Unregisters all the extractors
        void resetExtractors();
        //! Clean the data in the extractors
        void cleanExtractors();
        //! Adds a new extractor
        void registerExtractor(string name, FeatureExtractor* extr, bool used = true);
        //! Find an extractor
        FeatureExtractor* getExtractor(string name);

        //! Set up the extractors from an XML file
        bool setupPipeline(string filename);

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
        //! Removes the old features so that the newest remain
        void cleanOldFeatures(unsigned int newestCount);
        //! Get the dimension : sum of all the nbElems(i)
        unsigned int dimension() { return mDimension; }
        //! Get the real dimension : sum of all the nbElems we care (the values which are actually used for detection)
        unsigned int realDimension() { return mRealDimension; }
        //! Is this feature used for detection ?
        bool isUsed(unsigned int index);
        //! Get the name of the nth feature
        string name(unsigned int n);
        //! Get the id of the feature named so (-1 if not found)
        int getFeatureByName(string name);

        //! Handle audio chunks
        void useBuffer();
        //! End the computation
        void sequenceEnds();

        //! Callback called when the buffer has been used (the features are ready to be used)
        virtual void useFeatures() { ; }

        //! Mirrors

        //! Get the sampling frequency
        unsigned int samplingFrequency() { return mFrequency; }
        //! Get the playing time
        unsigned int getPlayingTime() { return playingTime(); }

    private:
        vector<pair<string, FeatureExtractor* > > mExtr; // TODO : it could be an hashtable
        vector<bool> mUsed;
        vector<float**> mFeatures;

        int mDimension;
        int mRealDimension;
        int mUsedExtractors;

        boost::mutex mSwitchLock;
        bool mComputed;
        libvlc_time_t mLastUpdateTime;
};

#endif
