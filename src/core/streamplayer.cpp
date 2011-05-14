#include "core/streamplayer.h"

#include <iostream>
using namespace std;

StreamPlayer::StreamPlayer() : mPlaying(false),
                               mMp(0),
                               mMedia(0),
                               mWatchThread(boost::bind(&StreamPlayer::watch, this)),
                               mDebugWritten(false)
{
    // Set up VLC
    char smem_options[256];
    mAudioData = 0;
    mAudioDataSize = 0;

    sprintf(smem_options, "#transcode{acodec=s16l}:smem{audio-postrender-callback=%lld,audio-prerender-callback=%lld,audio-data=%lld}",// "#transcode{acodec=s16l}:duplicate{dst=display,dst=smem{audio-postrender-callback=%lld,audio-prerender-callback=%lld,audio-data=%lld}}",
                           (long long int)(intptr_t)(void*)&handleStream, (long long int)(intptr_t)(void*)&prepareRender, (long long int)(intptr_t)(void*)this); // duplicate{dst=std,dst=smem{audio-postrender-callback=%lld,audio-data=%lld}}
    const char * const vlc_args[] = {
         //     "--extraintf=logger", //log anything
        //    "--verbose=2", //be much more verbose then normal for debugging purpose
              "--sout", smem_options //smem_options // Stream to memory
               };

    mVlcInstance = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);

    mMp = libvlc_media_player_new(mVlcInstance);
    libvlc_audio_set_volume (mMp,80);

    // Init data extraction
    mBuffer = new uint16_t[AUDIO_CHUNK_SIZE];
    mBufferSize = 0;
}

// Cleans up everything
StreamPlayer::~StreamPlayer()
{
    libvlc_media_player_stop(mMp);
    libvlc_media_player_release(mMp);

    libvlc_release(mVlcInstance);
}

// Start "playing" a stream (actually, start reading it and send it to the algorithms)
void StreamPlayer::play()
{
    if(!mMedia)
        mMedia = libvlc_media_new_path (mVlcInstance, mUrl.c_str());

    libvlc_media_player_set_media (mMp, mMedia);

    if(!mPlaying)
        libvlc_media_player_play (mMp);

    mPlaying = true;
    sleep(10);
}

// Time we've been playing this file, in ms (useful to evaluate how far did we compute)
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
        libvlc_media_player_stop(mMp);
}

// Get ready to render the stream to the buffer
void prepareRender( void* p_audio_data, uint8_t** pp_pcm_buffer , unsigned int size )
{
    StreamPlayer *sp = ((StreamPlayer*)p_audio_data);

    sp->mLock.lock();

    if(sp->mAudioDataSize < size)
    {
        if(sp->mAudioData != 0)
            delete sp->mAudioData;
        sp->mAudioData = new char[size];
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
    uint16_t* temp = StreamPlayer::convert8to16(p_pcm_buffer, size);
    size /= 2;

    // We implemented a mechanism that takes the data sent by libVLC and cut it into chunks
    // of the same size (a power of two) so that the algorithms can handle it in the right way
    while(remaining < size)
    {
        // Filling buffer
        while(sp->mBufferSize < AUDIO_CHUNK_SIZE && remaining < size)
        {
            sp->mBuffer[sp->mBufferSize] = temp[remaining];
            sp->mBufferSize++;
            remaining += channels;
        }

        if(sp->mBufferSize == AUDIO_CHUNK_SIZE)
        {
            // The buffer is sent to the "user"
            sp->useBuffer();

            // Emptying buffer
            sp->mBufferSize = 0;
        }
    }
    delete temp;

    sp->mLock.unlock();
}

// Wait for the computation to end
void StreamPlayer::watch()
{
    boost::posix_time::millisec waitTime(500);
    while(mMp == 0)
        boost::this_thread::sleep(waitTime);

    mPlaying = libvlc_media_player_is_playing(mMp);

    // How do we get out of here ? When mPlaying is set to false, this thread will be killed
    // TODO: change this, this may be the cause of our problems at the end of the computation
    while(1)
    {
        mPlayingLock.lock();
        if(mPlaying != libvlc_media_player_is_playing(mMp))
        {
            mPlaying = !mPlaying;
            if(!mPlaying)
                sequenceEnds();
        }
        mPlayingLock.unlock();
        boost::this_thread::sleep(waitTime);
    }
}

// Rewrite data
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

// Reduces the length of the array by computing local averages on it (kind of basic resampling)
uint16_t* StreamPlayer::average(uint16_t* source, int size, int passes, int scale)
{
    int factor = pow2(passes) * scale;
    uint16_t* dest = new uint16_t[(int)(size/factor)];

    for(int i = 0; i != size/factor; i++)
    {
        dest[i] = 0;
        for(int j = 0; j != factor; j++)
            dest[i] += source[i*factor+j];
        dest[i] /= factor;
    }

    return dest;
}

// Another basic resampling
void StreamPlayer::reduce(uint16_t* source, uint16_t* dest, int size, int passes, int scale)
{
    unsigned int chunkSize = pow2(passes);
    uint16_t min = pow2(16)-1, max = 0;
    for(unsigned int i = 0; i != size/chunkSize; ++i)
    {
        for(unsigned int j = 0; j != chunkSize; ++j)
        {
            if(source[i*chunkSize + j] > max)
                max = source[i*chunkSize + j];
            if(source[i*chunkSize + j] < min)
                min = source[i*chunkSize + j];
        }
        dest[2*i] = max/scale;
        dest[2*i + 1] = max/scale;
    }
}

// Dump the data to a file (useful for debugging processes)
void StreamPlayer::dumpStreamToFile16(uint16_t* source, int size)
{
    if(!mDumpFile.isOpen())
    {
        mDumpFile.setFileName("dumpedStream.txt");
        mDumpFile.open(QIODevice::WriteOnly);
    }

    std::stringstream buf;

    for(int i = 0; i != size; ++i)
        buf << i+1 << "\t" << source[i] << "\n";
    mDumpFile.write(buf.str().c_str());
}

// Dump two streams to a file
void StreamPlayer::dumpStreamToFile16x2(uint16_t* source, uint16_t* second, int size)
{
    if(!mDumpFile.isOpen())
    {
        mDumpFile.setFileName("dumpedStream.txt");
        mDumpFile.open(QIODevice::WriteOnly);
    }

    std::stringstream buf;

    for(int i = 0; i != size; ++i)
        buf << i+1 << "\t" << source[i] << "\t" << second[i%(size/2)] << "\n";
    mDumpFile.write(buf.str().c_str());
}

// Dump a stream as a sequence of bytes
void StreamPlayer::dumpStreamToFile8(uint8_t* source, int size)
{
    if(!mDumpFile.isOpen())
    {
        mDumpFile.setFileName("dumpedStream.txt");
        mDumpFile.open(QIODevice::WriteOnly);
    }

    uint16_t reference = (uint16_t)(-1);
    reference /= 2;
    std::stringstream buf;
    for(int i = 0; i != size/2; ++i)
    {
        buf << i+1 << "\t" << (int)source[i] << "\n";
        /*
        buf << (int)source[2*i] << " " << (int)((((int)source[2*i+1]) << 8) | source[2*i]) << "\n";
        buf << (int)source[2*i+1] << " " << (int)((((int)source[2*i+1]) << 8) | source[2*i]) << "\n";
        // */
    }
    mDumpFile.write(buf.str().c_str());
}

// Write a line to the debugging output
void StreamPlayer::writeLine(string line)
{
    if(!mDebugWritten)
    {
        if(!mDumpFile.isOpen())
        {
            mDumpFile.setFileName("dumpedStream.txt");
            mDumpFile.open(QIODevice::WriteOnly);
        }
        mDumpFile.write(line.c_str());
        mDebugWritten = true;
    }
}

// Power of two
inline int StreamPlayer::pow2(int n) { return (1 << n); }

