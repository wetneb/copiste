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
#include "filters/memory.h"
#include "filters/compare.h"
#include "filters/distance.h"

using namespace std;


/**
 * \class SoundAnalyser
 * \brief Main class that sets up and runs a pipeline
 *
 * A pipeline is a network of features and filters connected to the input stream.
 * 
 * Some explanations about the behaviour of the class :
 *
 * ## Features, elements ##
 *
 * A feature is a set of values computed by an algorithm. The number of values it outputs
 * on a single audio chunk is the number of elements.
 *
 * ## Used features, Dimension vs real dimension ##
 *
 * In the pipeline, some features are labelled as "used" : it means they are sent
 * to the application to be used for classification, visualization, etc. The others
 * are not outputted directly and can only be used as input for filters.
 * The "real dimension" is the number of used features (x their number of elements).
 * The "dimension" is the number of computed features and filters (again, x their number
 * of elements).
 *
 * Note : the feature named _spectrum is usually not labelled as "used" but is displayed
 * by FeatureDrawer.
 *
 * ## Normalization ##
 *
 * Forces the outputs of the features (and filters) to be in a given range.
 *
 */
class SoundAnalyser : protected StreamPlayer
{
    public:
        //! Sets up a new sound analyser
        SoundAnalyser(bool live = false);
        //! Destructor.
        ~SoundAnalyser();

        /// Extractors management

        //! Unregisters all the extractors
        void resetExtractors();
        //! Clean the data in the extractors
        void cleanExtractors();
        //! Adds a new extractor
        void registerExtractor(string name, FeatureExtractor* extr, bool used = true, bool drawLines = true);
        //! Find an extractor
        FeatureExtractor* getExtractor(string name);

        /// Setup

        //! Set up the extractors from an XML file
        bool setupPipeline(string filename);
        //! Set up unified bounds for all features
        void setNormalization(float min, float max);
        //! Compute the features of a given file
        bool compute(string url);
        //! Wait up to the end of the computation
        void waitComputed();

        /// Features management : sizes

        //! Return the number of different features we computed
        unsigned int nbFeatures() { return mExtr.size(); }
        //! Return the number of elements of the nth feature
        unsigned int nbElems(unsigned int n);
        //! Return the number of sample we've been computing
        unsigned int nbSamples() { return mFeatures.size(); }
        //! Get the dimension : sum of all the nbElems(i)
        unsigned int dimension() { return mDimension; }
        //! Get the real dimension : sum of all the nbElems we care (the values which are actually used for detection)
        unsigned int realDimension() { return mRealDimension; }

        // Features management : data

        //! Return the array of features for the nth sample
        double** features(unsigned int n);
        //! Clears the features
        void clearFeatures();
        //! Removes the old features so that the newest remain
        void cleanOldFeatures(unsigned int newestCount);

        /// Features management : individual properties

        //! Is this feature used for detection ?
        bool isUsed(unsigned int index);
        //! Should this feature be drawn as a lines or a shade of grey ?
        bool isDrawnWithLines(unsigned int index);
        //! Get the name of the nth feature
        string name(unsigned int n);
        //! Get the id of the feature named so (-1 if not found)
        int getFeatureByName(string name);

        /// Callbacks

        //! Handle audio chunks
        void useBuffer();
        //! End the computation
        void sequenceEnds();
        //! Callback called when the buffer has been used (the features are ready to be used)
        virtual void useFeatures() { ; }

        //! Mirrors

        //! Get the sampling frequency
        unsigned int samplingFrequency() { return mFrequency; }

    private:
        vector<pair<string, FeatureExtractor* > > mExtr; // TODO : it could be an hashtable
        vector<bool> mUsed;
        vector<bool> mDrawLines;
        vector<double**> mFeatures;

        int mDimension;
        int mRealDimension;
        int mUsedExtractors;

        bool mNormalize;
        float mOverlapping;
        float mMinFeature;
        float mMaxFeature;

        boost::mutex mSwitchLock;
        bool mComputed;
        libvlc_time_t mLastUpdateTime;
};

#endif
