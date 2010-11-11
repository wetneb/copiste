#include "core/streamplayer.h"

#include <iostream>
using namespace std;


QMutex mAudioMutex;
char *audioData;

StreamPlayer::StreamPlayer() : mPlaying(false),
                               mMedia(0),
                               mGraphique(0),
                               mSpectre(0),
                               mDebugWritten(false)
{
    // Initialisation de VLC
    char smem_options[256];
    audioData = new char[128];

    sprintf(smem_options, "#transcode{acodec=s16l}:smem{audio-postrender-callback=%lld,audio-prerender-callback=%lld,audio-data=%lld}",// "#transcode{acodec=s16l}:duplicate{dst=display,dst=smem{audio-postrender-callback=%lld,audio-prerender-callback=%lld,audio-data=%lld}}",
                            (long long int)(intptr_t)(void*)&handleStream, (long long int)(intptr_t)(void*)&prepareRender, (long long int)(intptr_t)(void*)this); // duplicate{dst=std,dst=smem{audio-postrender-callback=%lld,audio-data=%lld}}
    const char * const vlc_args[] = {
              "-I", "dummy", /* Don't use any interface */
              "--ignore-config", /* Don't use VLC's config */
              "--extraintf=logger", //log anything
              "--verbose=2", //be much more verbose then normal for debugging purpose
              "--sout", smem_options //smem_options // Stream to memory
               };

    mVlcInstance = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);

    mMp = libvlc_media_player_new(mVlcInstance);
    setVol(80);

    cout << "Audio chain : " << smem_options << endl;

    // Connections pour la lecture
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(update()));

    // Lancement
    mTimer.start(100);
}

StreamPlayer::~StreamPlayer()
{
    libvlc_media_player_stop(mMp);
    libvlc_media_player_release(mMp);

    libvlc_release(mVlcInstance);
}

void StreamPlayer::update()
{
    if(!mPlaying)
        return;

        // TBC
}

void StreamPlayer::play()
{
    if(!mMedia)
    {
        cout << "Big Begin ######" << endl;
        mMedia = libvlc_media_new_path (mVlcInstance, mUrl.toAscii());
    }
    libvlc_media_player_set_media (mMp, mMedia);
    cout << "Out of the block ####" <<endl;
    if(!mPlaying)
        libvlc_media_player_play (mMp);
    cout << "Startted to play #######" << endl;
    mPlaying = true;
}

void StreamPlayer::stop()
{
    libvlc_media_player_stop(mMp);
    mPlaying = false;

    // À supprimer !
    mDumpFile.close();
}

void StreamPlayer::setVol(int vol)
{
    libvlc_audio_set_volume (mMp,vol);
}

void prepareRender( void* p_audio_data, uint8_t** pp_pcm_buffer , unsigned int size )
{
    ((StreamPlayer*)p_audio_data)->mLock.lock();

    *pp_pcm_buffer = (uint8_t*)audioData;
}

void handleStream(void* p_audio_data, uint8_t* p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, unsigned int size, int64_t pts )
{
    // cout << "handleStream" << endl;
    //if(nb_samples)
    //   cout << "NbSamples : " << nb_samples << ", Channels : " << channels << " Size : " << size << ", bits per sample : " << bits_per_sample << ", pts : " << pts << endl;


    std::stringstream buf;
    buf << "NbSamples : " << nb_samples << ", Channels : " << channels << " Size : " << size << ", bits per sample : " << bits_per_sample << ", pts : " << pts << ", rate " << rate << endl;
    ((StreamPlayer*)p_audio_data)->writeLine(buf.str());

    uint16_t* temp = StreamPlayer::convert8to16(p_pcm_buffer, size);

    uint16_t* pcm_buffer = new uint16_t[size/64];
     StreamPlayer::reduce(temp, pcm_buffer, size, 9, 90);
     //cout << ZCR(temp,size/2) << endl;
    //StreamPlayer::addOffset(pcm_buffer, pcm_buffer, size/128, 10000);
     // StreamPlayer::average(temp, size, 7, 2);
    // ((StreamPlayer*)p_audio_data)->dumpStreamToFile16(temp, size/2);


    // /*
    if(((StreamPlayer*)p_audio_data)->graphiqueOnde())
        ((StreamPlayer*)p_audio_data)->graphiqueOnde()->appendData((uint16_t*)pcm_buffer, size/512);
    //*/

    uint16_t* spectrum = naiveDFT(temp, 9);
    ((StreamPlayer*)p_audio_data)->dumpStreamToFile16(spectrum, pow(2, 9));
    ((StreamPlayer*)p_audio_data)->dumpStreamToFile16(temp, pow(2,9));
    /*
    uint16_t* reducedSpectrum = new uint16_t[128];
    StreamPlayer::reduce(spectrum, reducedSpectrum, 512, 2, true);
//    uint16_t* reducedSpectrum = StreamPlayer::average(spectrum, size
    cout << "Frenquencies generated." << endl;

    if(((StreamPlayer*)p_audio_data)->graphiqueSpectre())
        ((StreamPlayer*)p_audio_data)->graphiqueSpectre()->setData((uint16_t*)reducedSpectrum, 128);

    delete spectrum; */

    //delete temp;
    //delete pcm_buffer;

    ((StreamPlayer*)p_audio_data)->mLock.unlock();
}

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

void StreamPlayer::addOffset(uint16_t* source, uint16_t* dest, int size, int offset)
{
    for(int i = 0; i != size; ++i)
    {
        dest[i] = source[i] + offset;
    }
}

uint16_t* StreamPlayer::average(uint16_t* source, int size, int passes, int scale)
{
    int factor = pow(2,passes) * scale;
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


void StreamPlayer::reduce(uint16_t* source, uint16_t* dest, int size, int passes, int scale)
{
    unsigned int chunkSize = pow(2,passes);
    uint16_t min = pow(2,16)-1, max = 0;
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

void StreamPlayer::dumpStreamToFile16(uint16_t* source, int size)
{
    if(!mDumpFile.isOpen())
    {
        mDumpFile.setFileName("dumpedStream.txt");
        mDumpFile.open(QIODevice::WriteOnly);
    }

    std::stringstream buf;
    for(int i = 0; i != size; ++i)
        buf << source[i] << "\n";
    mDumpFile.write(buf.str().c_str());
}

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
        buf << (int)source[2*i] << " " << (int)((((int)source[2*i+1]) << 8) | source[2*i]) << "\n";
        buf << (int)source[2*i+1] << " " << (int)((((int)source[2*i+1]) << 8) | source[2*i]) << "\n";
    }
    mDumpFile.write(buf.str().c_str());
}

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

/*
void
VideoClipWorkflow::lock( VideoClipWorkflow *cw, void **pp_ret, int size )
{
    Q_UNUSED( size );
    LightVideoFrame*    lvf = NULL;

    cw->m_renderLock->lock();
    if ( cw->m_availableBuffers.isEmpty() == true )
    {
        lvf = new LightVideoFrame( cw->m_width, cw->m_height );
    }
    else
        lvf = cw->m_availableBuffers.dequeue();
    cw->m_computedBuffers.enqueue( lvf );
    *pp_ret = (*(lvf))->frame.octets;
}

void
VideoClipWorkflow::unlock( VideoClipWorkflow *cw, void *buffer, int width,
                           int height, int bpp, int size, qint64 pts )
{
    Q_UNUSED( buffer );
    Q_UNUSED( width );
    Q_UNUSED( height );
    Q_UNUSED( bpp );
    Q_UNUSED( size );

    cw->computePtsDiff( pts );
    LightVideoFrame     *lvf = cw->m_computedBuffers.last();
    (*(lvf))->ptsDiff = cw->m_currentPts - cw->m_previousPts;
    cw->commonUnlock();
    cw->m_renderWaitCond->wakeAll();
    cw->m_renderLock->unlock();
}
*/
