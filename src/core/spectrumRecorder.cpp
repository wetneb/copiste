#include "core/spectrumRecorder.h"

//! Default constructor. Sets up the computation.
SpectrumRecorder::SpectrumRecorder(int windowSize, int sampleFrequency, int maxHeight, int scale) :
    mOut(windowSize/2, maxHeight, QImage::Format_ARGB32)
{
    mWindowSize = windowSize;
    mSampleFrequency = sampleFrequency;
    mMaxHeight = maxHeight;
    mScale = scale;

    mCurrentRow = 0;
    mComputed = true;
}

//! Computes the spectrum
bool SpectrumRecorder::compute(string file)
{
    mOut.fill(0);
    mComputed = false;

    setUrl(file);
    play();

    boost::posix_time::seconds waitTime(1);

    mSwitchLock.lock();
    while(!mComputed)
    {
        mSwitchLock.unlock();
        boost::this_thread::sleep(waitTime);
        mSwitchLock.lock();
    }
    mSwitchLock.unlock();

    return true;
}

//! Write the image (internal)
void SpectrumRecorder::useBuffer()
{
    // libvlc_media_player_get_length/time
    if(mCurrentRow < mMaxHeight)
    {
        mExtr.extract(mBuffer, AUDIO_CHUNK_SIZE);
        mExtr.normalize(255);
        for(int i = 0; i < mWindowSize / 2; ++i)
            mOut.setPixel(i, mCurrentRow, qRgb(0, mExtr.spectrum()[i], 0));
        mCurrentRow++;
    }
    else
    {
        sequenceEnds();
    }
}

void SpectrumRecorder::sequenceEnds()
{
    cout << "\e[F\e[KDone." << endl;

    mSwitchLock.lock();
    mComputed = true;
    mSwitchLock.unlock();

    stop();
}

//! Write down the image
void SpectrumRecorder::writeToFile(string file)
{
    mOut.save(file.c_str());
}
