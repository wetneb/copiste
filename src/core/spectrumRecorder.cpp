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

SpectrumRecorder::~SpectrumRecorder()
{
    ;
}

//! Computes the spectrum
bool SpectrumRecorder::compute(string file)
{
    mOut.fill(0);
    mComputed = false;

    setUrl(file);
    play();

    boost::posix_time::seconds waitTime(1);

    //mSwitchLock.lock();
    while(!mComputed)
    {
        //mSwitchLock.unlock();
        boost::this_thread::sleep(waitTime);
        //mSwitchLock.lock();
    }
    //mSwitchLock.unlock();

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
        int max = 0;
        for(int i = 0; i < mWindowSize / 2; ++i)
        {
            int val = mExtr.spectrum()[i];
            if(val > 255)
            {
                val = 255;
                if(val > max)
                    max = val;
            }
            mOut.setPixel(i, mCurrentRow, qRgb(val, val, val));
        }
        if(max > 255)
            cout << "! > " << max << endl;

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

    //mSwitchLock.lock();
    mComputed = true;
    //mSwitchLock.unlock();

    stop();
}

//! Write down the image
void SpectrumRecorder::writeToFile(string file)
{
    mOut.save(file.c_str());
}
