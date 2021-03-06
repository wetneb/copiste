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


#ifndef INCLUDEDSTREAMPLAYERH
#define INCLUDEDSTREAMPLAYERH

#include <vlc/vlc.h>

//! TODO Remove this dependency from Qt (VLC implements mutexes, boost too)
#include <QMutex>

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>

#include "features/spectrum.h"

#define DEFAULT_AUDIO_CHUNK_SIZE 4096
#define VLC_DEFAULT_VOLUME 80

using namespace std;

class StreamPlayer;

// Callbacks audio
void handleStream(void* p_audio_data, uint8_t* p_pcm_buffer, unsigned int channels, unsigned int rate,
                  unsigned int nb_samples, unsigned int bits_per_sample, unsigned int size, int64_t pts);
void prepareRender(void* p_audio_data, uint8_t** pp_pcm_buffer , unsigned int size);

/**
 * \class StreamPlayer
 * \brief Manages stream decoding and sends it to the computing part of the application
 * This class is intended to be rewritten for more specific usages (spectrum analysis, feature extraction, aso.).
 * Its the interface between libVLC (which reads the media, decodes, resamples, and does all the hard DSP) and
 * the using of the data.
 *
 * ## Overlapping ##
 *
 * When using overlapping, two audio chunks have some audio data in common :
 * their size time the overlapping factor.
 */
class StreamPlayer
{
    public:
        // Set up functions

        //! Default constructor. Starts VLC instance.
        StreamPlayer(bool live = false, bool verbose = true);
        //! Default destructor. Frees memory and closes VLC.
        ~StreamPlayer();
        //! Returns URL of the currently playing stream
        string url() { return mUrl; }
        //! Defines the URL of the stream to play
        void setUrl(string url) { mUrl = url; }
        //! Turns on / off audio chunk overlapping
        void setOverlapping(float factor);
        //! Changes the audio chunk size
        void setChunkSize(int size);
        //! Get the current chunk size
        int chunkSize() { return mChunkSize; }

        //! Plays the media
        void play();
        //! Pause the playing
        void togglePauseResume();
        //! Is the stream paused ?
        bool isPaused(); 
        //! Returns the playing time
        libvlc_time_t playingTime();
        //! Returns the total time (played + to be played)
        libvlc_time_t totalTime();
        //! Stops playing
        void stop();
        //! Are we playing a live stream ?
        bool isLive() { return mLive; }
        //! Changes the volume (between 0 and 100)
        void setVolume(int vol);
        //! Changes the playback rate (1.0 : normal)
        void setRate(float rate);
        //! Gets the current volume
        int volume();

        // Computing functions : designed to be overloaded by the user

        //! Callback called when we start playing a file
        virtual void sequenceStarts() { ; }
        //! Callback called when the data is ready in the buffer (miam).
        // The user don't have to manage the memory.
        virtual void useBuffer() { ; }
        //! Callback called when the file ended
        virtual void sequenceEnds() { ; }

        // Handling functions

        //! Get 2^n
        static int pow2(int n);

        //! Watching thread
        void watch();

        //! Buffer acces (for the user)
        inline int16_t buffer(int i);
        //! Current buffer size (internal)
        inline int bufferSize();
        //! Fill buffer with a new value (internal)
        inline void fillBuffer(int16_t value);
        //! Flush buffer (just keep the overlapping part)
        inline void flushBuffer();

        //! Feature extraction : those variables need to be public
        // (I know, I can write accessors...)
        int16_t* mBuffer;
        QMutex mLock;

        // Prerender callback
        char* mAudioData;
        unsigned int mAudioDataSize;
        unsigned int mBufferSize;
        unsigned int mFrequency; // detected from VLC

    protected:
        bool mVerbose;
        int mChannels;

    private:
        // Parameters
        string mUrl;
        bool mPlaying;
        bool mLive;
        float mOverlapping;
        float mRate;
        
        int mChunkSize;
        int mFramesOverlap;

        // VLC
        libvlc_instance_t *mVlcInstance;
        libvlc_media_player_t *mMp;
        libvlc_media_t *mMedia;

        boost::thread mWatchThread;
        boost::mutex mPlayingLock;
        bool mPaused;

        friend
        void handleStream(void*, uint8_t*, unsigned int, unsigned int,
                  unsigned int, unsigned int, unsigned int, int64_t);
        friend
        void prepareRender(void*, uint8_t**, unsigned int);
};

/**
 * \mainpage
 *
 * \section Summary
 * Copiste may become (one day) a radio ripper featuring metadata extraction
 * and audio recognition using learning algorithms. Currently, it allows
 * music / speech detection on audio files as well as live radio streams.
 * The core software relies on a large number of XML files and is therefore
 * easy to hack on the fly.
 *
 * The computing part of the project relies on libvlc, which provides a large
 * set of decoders and a powerful streaming pipeline. The other dependencies
 * are Qt and boost. The project can be built either with SCons or with CodeBlocks.
 *
 * \section Tools
 * \subsection nnat nnat : Neural Network Analysis Tool
 * This tool takes a neural network described in an XML format
 * (see the networks directory for examples) and trains it to
 * fit a given corpus, described in another XML format (see the
 * corpus directory for examples). It can also evaluate how good
 * a network fits a corpus, and provides a small graphical interface
 * that can be used to visualize 2D networks.
 *
 * \subsection featdraw featdraw : Feature Drawer
 * This tool takes an audio file and draws the variations of some
 * features in this file. The features are described by a pipeline
 * (XML file, see the pipeline directory), allowing the user to
 * change on the fly how features are computed.
 *
 * \subsection mkcorpus mkcorpus : Make Corpus
 * This tool uses a pipeline and a set of audio files to create
 * a corpus (as in nnat) where the features are written.
 * It uses another XML file, which defines the different audio
 * classes.
 *
 * \subsection specdraw specdraw : Draw Spectrum
 * This tool just takes as input an audio file and draws the
 * variations of the frequency spectrum during the time.
 *
 * \subsection classify classify : Classify an audio file
 * This program takes an audio file, a pipeline and a network, and
 * says what class the file belongs to.
 *
 * \subsection live live : Live stream classification
 * This program reads a live stream (e.g. a radio), and shows the
 * variations of the features and the classification of the stream.
 *
 * \subsection fingerprint fingerprint : Stream classification using fingerprints
 * This program can either train or use an Hidden Markov Model
 * to classify sounds using fingerprints (which are computed by libchromaprint).
 * It is currently under development.
 *
 */

#endif

