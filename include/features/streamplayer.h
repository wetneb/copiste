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
#include <QFile> // to be removed !
#include <QMutex>

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
 */
class StreamPlayer
{
    public:
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

        //! Converts an array of uint8_t to another array of uint16_t (assuming the values are coded on two bytes)
        static uint16_t* convert8to16(const uint8_t* source, int size);
        //! Computes the average value of the stream, on a given frequency
        static uint16_t* average(uint16_t* source, int size, int passes, int scale= 1);
        //! Reduces the stream, keeping only the extreme values
        static void reduce(uint16_t* source, uint16_t* dest, int size, int passes, int scale=1);
        //! Adds an offset to each value of the array
        static void addOffset(uint16_t* source, uint16_t* dest, int size, int offset);
        //! Dumps values to a file (useful in a debugging process) (16 bit version)
        void dumpStreamToFile16(uint16_t* source, int size);
        void dumpStreamToFile16x2(uint16_t* source, uint16_t* second, int size);
        //! Dumps values to a file (8 bit version)
        void dumpStreamToFile8(uint8_t* source, int size);
        //! Write a line to the dump file
        void writeLine(std::string line);


        //! Feature extraction : those variables need to be public (I know, I can write accessors...)
        uint16_t* mBuffer;
        int mBufferSize;
        float* mFeaturesValues;
        int mValuesSize;
        QMutex mLock;

        // Prerender callback
        char* mAudioData;
        unsigned int mAudioDataSize;

        // Features
        SpectrumExtr mSpecExtr;

    private:
        void update();

    private:
        // Paramètres
        string mUrl;
        bool mPlaying;

        // VLC
        libvlc_instance_t *mVlcInstance;
        libvlc_media_player_t *mMp;
        libvlc_media_t *mMedia;

        // Debug
        QFile mDumpFile;
        bool mDebugWritten;
};

#endif
