#ifndef INCLUDEDSTREAMCATCHER
#define INCLUDEDSTREAMCATCHER

#include <QThread>
#include <vlc/vlc.h>

class StreamCatcher : public QThread
{
    public:
        StreamCatcher(QObject *parent = 0) : QThread(parent), mDataSet(false) { ; }

        void setAudioChunk(void* p_audio_data, uint8_t* p_pcm_buffer, unsigned int channels,
                           unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample,
                           unsigned int size, int64_t pts );

        void run();

    private:
        bool mDataSet;

        void* mAudioData;
        uint8_t* mPCMBuffer;
        unsigned int mChannels;
        unsigned int mRate;
        unsigned int mNbSamples;
        unsigned int mBitsPerSample;
        unsigned int mSize;
        int64_t mPTS;
};


#endif
