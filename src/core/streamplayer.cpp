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


#include "core/streamplayer.h"

#include <iostream>
using namespace std;


// This class is just a small hack, everything is hardcoded

StreamPlayer::StreamPlayer(bool live, bool verbose) : mVerbose(verbose),
                                                      mOverlapping(0),
                                                      mChunkSize(DEFAULT_AUDIO_CHUNK_SIZE),
                                                      mFramesOverlap(0),
                                                      mMp(0),
                                                      mMedia(0)
{
    // Set up VLC
    mLive = live;
    char smemOptions[256];
    mAudioData = 0;
    mAudioDataSize = 0;

    if(live)
    {
        sprintf(smemOptions, "#transcode{acodec=s16l}:duplicate{dst=display,dst=smem"
         "{audio-postrender-callback=%lld,audio-prerender-callback=%lld,audio-data=%lld}}",
                           (long long int)(intptr_t)(void*)&handleStream,
                           (long long int)(intptr_t)(void*)&prepareRender, 
                           (long long int)(intptr_t)(void*)this);
         // "#transcode{acodec=s16l}:duplicate{dst=display,dst=smem
         // {audio-postrender-callback=%lld,audio-prerender-callback=%lld,audio-data=%lld}}",
    }
    else
        sprintf(smemOptions, "#transcode{acodec=s16l}:smem"
                "{audio-postrender-callback=%lld,audio-prerender-callback=%lld,audio-data=%lld}", 
                (long long int)(intptr_t)(void*)&handleStream,
                (long long int)(intptr_t)(void*)&prepareRender,
                (long long int)(intptr_t)(void*)this);
        // "#transcode{acodec=s16l}:duplicate{dst=display,dst=smem
        // {audio-postrender-callback=%lld,audio-prerender-callback=%lld,audio-data=%lld}}",

    const char stringSout[] = "--sout", stringNoSoutSmemTimeSync[] = "--no-sout-smem-time-sync";
    const char * const vlcArgs[] = {
        stringSout,
        smemOptions,
        stringNoSoutSmemTimeSync };

    mVlcInstance = libvlc_new((live ? 2 : 3), vlcArgs); // deleted in the destructor

    mMp = libvlc_media_player_new(mVlcInstance); // deleted in the destructor
    libvlc_audio_set_volume (mMp,VLC_DEFAULT_VOLUME);
}

// Cleans up everything
StreamPlayer::~StreamPlayer()
{
    //libvlc_media_player_stop(mMp);
    libvlc_media_player_release(mMp);

    libvlc_release(mVlcInstance);

    delete [] mAudioData;
}

// Start "playing" a stream (actually, start reading it and send it to the algorithms)
void StreamPlayer::play()
{
    if(mMedia)
        libvlc_media_release(mMedia);
    mMedia = libvlc_media_new_path (mVlcInstance, mUrl.c_str());

    mFramesOverlap = mOverlapping * mChunkSize;
    libvlc_media_player_set_media (mMp, mMedia);

    libvlc_media_player_play (mMp);

    mPlaying = true;
    boost::thread watchThread(boost::bind(&StreamPlayer::watch, this));
    watchThread.detach();
}

// Time we've been playing this file, in ms (useful to evaluate how far we computed)
libvlc_time_t StreamPlayer::playingTime()
{
    return libvlc_media_player_get_time(mMp);
}

// Total length of the current file, in ms
libvlc_time_t StreamPlayer::totalTime()
{
    return libvlc_media_player_get_length(mMp);
}

// Stop computing
void StreamPlayer::stop()
{
    if(mPlaying)
    {
        libvlc_media_player_stop(mMp);
        mWatchThread.interrupt();
    }
}

// Get ready to render the stream to the buffer
void prepareRender( void* p_audio_data, uint8_t** pp_pcm_buffer , unsigned int size )
{
    StreamPlayer *sp = ((StreamPlayer*)p_audio_data);

    sp->mLock.lock();

    if(sp->mAudioDataSize < size)
    {
        delete [] sp->mAudioData;
        sp->mAudioData = new char[size]; // Deleted in the destructor
    }
    *pp_pcm_buffer = (uint8_t*)(sp->mAudioData);
}

void handleStream(void* p_audio_data, uint8_t* p_pcm_buffer, unsigned int channels, unsigned int rate,
                  unsigned int nb_samples, unsigned int bits_per_sample, unsigned int size, int64_t pts )
{
    unsigned int remaining = 0;
    StreamPlayer *sp = ((StreamPlayer*)p_audio_data);

    // The data is sent to us as bytes, but encoded on 2 bytes
    // TODO: dynamicly check that this is the case and that we're not mishandling the data
    // TODO: stereo support
    // TODO: allocate temp only once and send the address to convert8to16
    uint16_t* temp = StreamPlayer::convert8to16(p_pcm_buffer, size);
    size /= 2;

    // We implemented a mechanism that takes the data sent by libVLC and cut it into chunks
    // of the same size (a power of two) so that the algorithms can handle it in the right way
    while(remaining < size)
    {
        // \todo Faudrait-il plutôt utiliser memcpy & co ? J'y connais rien à ces trucs-là…
        // Filling buffer
        while(sp->bufferSize() < sp->chunkSize() && remaining < size)
        {
            sp->fillBuffer(temp[remaining]);
            remaining += channels;
        }

        if(sp->bufferSize() >= sp->chunkSize())
        {
            // The buffer is sent to the "user"
            sp->useBuffer();

            // Emptying buffer
            sp->flushBuffer();
        }
    }
    delete [] temp;

    // Update the frequency if needed
    if(rate != sp->mFrequency)
        sp->mFrequency = rate;

    sp->mLock.unlock();
}

// Wait for the computation to end
void StreamPlayer::watch()
{
    boost::posix_time::millisec waitTime(100);
    bool seenPlaying = false;

    mPlayingLock.lock();
    while(libvlc_media_player_is_playing(mMp) || !seenPlaying)
    {
        if(!seenPlaying && libvlc_media_player_is_playing(mMp))
            seenPlaying = true;
        mPlayingLock.unlock();

        boost::this_thread::sleep(waitTime);

        mPlayingLock.lock();
    }
    mPlayingLock.unlock();

    sequenceEnds();
}

// Rewrite data. The returned array has to be deleted by the user
uint16_t* StreamPlayer::convert8to16(const uint8_t* source, int size)
{
    uint16_t* dest = new uint16_t[size/2];
    uint16_t buf, reference = (uint16_t)(-1);
    reference /= 2;
    for(int i = 0; i != size/2; i++)
    {
        buf = source[i*2+1];
        buf = buf << 8;
        buf = buf | source[i*2];
        if(buf > reference)
            buf -= reference;
        else buf += reference;
        dest[i] = buf;
    }
    return dest;
}

// Unused function that adds an offset to an array
void StreamPlayer::addOffset(uint16_t* source, uint16_t* dest, int size, int offset)
{
    for(int i = 0; i != size; ++i)
    {
        dest[i] = source[i] + offset;
    }
}

uint16_t StreamPlayer::buffer(int i)
{
    return mBuffer.at(i);
}

int StreamPlayer::bufferSize()
{
    return mBuffer.size();
}

void StreamPlayer::fillBuffer(uint16_t value)
{
    mBuffer.push_back(value);
}

void StreamPlayer::flushBuffer()
{
    mBuffer.erase(mBuffer.begin(), mBuffer.begin() + (mChunkSize - mFramesOverlap));
}

void StreamPlayer::setOverlapping(float factor)
{
    if(0 <= factor && factor <= 0.99)
        mOverlapping = factor;
    else
    {
        mOverlapping = 0;
        std::cerr << "Warning : invalid overlapping factor (" << factor << ")"
                    ", overlapping disabled." << std::endl;
    }
}

void StreamPlayer::setChunkSize(int size)
{
    if(size >= 1)
    {
        mBuffer.clear();
        mChunkSize = size;
        mFramesOverlap = size * mOverlapping;
    }
}

void StreamPlayer::setVolume(int vol)
{
    if(vol % 20 == 0)
        std::cout << "\nVol set to " << vol << std::endl;
    libvlc_audio_set_volume (mMp,vol);
}

int StreamPlayer::volume()
{
     return libvlc_audio_get_volume(mMp);
}

// Power of two
inline int StreamPlayer::pow2(int n) { return (1 << n); }

