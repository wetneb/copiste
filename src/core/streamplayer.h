#ifndef INCLUDEDSTREAMPLAYERH
#define INCLUDEDSTREAMPLAYERH

#include <vlc/vlc.h>

//! TODO Remove these dependencies from Qt (QFile shouldn't be a problem, and VLC implements mutexes)
#include <QFile> // to be removed !
#include <QMutex>

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>

#include "features/spectrum.h"

#define AUDIO_CHUNK_SIZE 2048

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
 * TODO : add parameters : enable to set verbosity (display progress or not)
 * TODO                    enable to compute directly or not
 * TODO                    enable to send the stream to speakers or not
 * TODO                    enable to…
 */
class StreamPlayer
{
    public:
        // Set up functions

        //! Default constructor. Starts VLC instance.
        StreamPlayer();
        //! Default destructor. Frees memory and closes VLC.
        ~StreamPlayer();
        //! Returns URL of the currently playing stream
        string url() { return mUrl; }
        //! Defines the URL of the stream to play
        void setUrl(string url) { mUrl = url; }

        //! Plays the media
        void play();
        //! Returns the playing time
        libvlc_time_t playingTime();
        //! Returns the total time (played + to be played)
        libvlc_time_t totalTime();
        //! Stops playing
        void stop();

        // Computing functions : designed to be overloaded by the user

        //! Callback called when we start playing a file
        virtual void sequenceStarts() { ; }
        //! Callback called when the data is ready in the buffer (miam). The user don't have to manage the memory.
        virtual void useBuffer() { ; }
        //! Callback called when the file ended
        virtual void sequenceEnds() { ; }

        // Handling functions

        /**
         * \brief Converts an array of uint8_t to another array of uint16_t (assuming the values are coded on two bytes)
         * The returned pointer has to be deleted by the user.
         */
        static uint16_t* convert8to16(const uint8_t* source, int size);
        /**
         * \brief Computes the average value of the stream, on a given frequency
         * The returned pointer has to be deleted by the user.
         */
        static uint16_t* average(uint16_t* source, int size, int passes, int scale= 1);
        //! Reduces the stream, keeping only the extreme values
        static void reduce(uint16_t* source, uint16_t* dest, int size, int passes, int scale=1);
        //! Adds an offset to each value of the array
        static void addOffset(uint16_t* source, uint16_t* dest, int size, int offset);
        //! Get 2^n
        static int pow2(int n);

        //! Watching thread
        void watch();

        //! Feature extraction : those variables need to be public (I know, I can write accessors...)
        uint16_t* mBuffer;
        int mBufferSize;
        QMutex mLock;

        // Prerender callback
        char* mAudioData;
        unsigned int mAudioDataSize;
        unsigned int mFrequency; // detected from VLC

    private:
        // Paramètres
        string mUrl;
        bool mPlaying;

        // VLC
        libvlc_instance_t *mVlcInstance;
        libvlc_media_player_t *mMp;
        libvlc_media_t *mMedia;

        boost::thread mWatchThread;
        boost::mutex mPlayingLock;
};

#endif
