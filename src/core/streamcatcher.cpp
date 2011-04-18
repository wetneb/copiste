#include "core/streamcatcher.h"

#include "core/streamplayer.h"

void StreamCatcher::setAudioChunk(void* p_audio_data, uint8_t* p_pcm_buffer, unsigned int channels,
                                   unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample,
                                   unsigned int size, int64_t pts )
{
    if(wait())
    {
        mAudioData = (StreamPlayer*)p_audio_data;
        mPCMBuffer = p_pcm_buffer;
        mChannels = channels;
        mRate = rate;
        mNbSamples = nb_samples;
        mBitsPerSample = bits_per_sample;
        mSize = size;
        mPTS = pts;

        mDataSet = true;
    }
}

void StreamCatcher::run()
{
    if(mDataSet)
    {
        if(mSize >= 1024) {
        //! Debug section

        // std::stringstream buf;
        // buf << "NbSamples : " << nb_samples << ", Channels : " << channels << " Size : " << size << ", bits per sample : " << bits_per_sample << ", pts : " << pts << ", rate " << rate << endl;
        // ((StreamPlayer*)p_audio_data)->writeLine(buf.str());


        //! Conversion section
        uint16_t* temp = StreamPlayer::convert8to16(mPCMBuffer, mSize);
        std::cout << mSize <<".\n";
        //uint16_t* pcm_buffer = new uint16_t[mSize/64];
        //StreamPlayer::reduce(temp, pcm_buffer, mSize, 9, 90);
        //StreamPlayer::addOffset(pcm_buffer, pcm_buffer, size/128, 10000);
         // StreamPlayer::average(temp, size, 7, 2);
        // ((StreamPlayer*)p_audio_data)->dumpStreamToFile16(temp, size/2);


        //! Rendering section
         /*
        if(((StreamPlayer*)mAudioData)->graphiqueOnde())
            ((StreamPlayer*)mAudioData)->graphiqueOnde()->appendData((uint16_t*)mPCMBuffer, mSize/512);
        //*/


        //! Analysis section
        //std::cout << ZCR(temp,mSize/2) << std::endl;
        uint16_t* spectrum = naiveDFT(temp, 10);
        //########################################
        mAudioData->dumpStreamToFile16x2(temp, spectrum, 1024);
        //((StreamPlayer*)p_audio_data)->dumpStreamToFile16(temp, pow(2,9));
        //########################################


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
        }
        ((StreamPlayer*)mAudioData)->mLock.unlock();
    }
}
