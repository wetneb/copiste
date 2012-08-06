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


#ifndef INCLUDED_SPECTRUMRECORDERH
#define INCLUDED_SPECTRUMRECORDERH

#include <QImage>
#include <boost/thread.hpp>

#include "core/streamplayer.h"
#include "features/spectrum.h"

/**
 * \class SpectrumRecorder
 * \brief A class which creates an image of the spectrum of a given file
 * This is not used to compute features, that's just a tool.
 */
class SpectrumRecorder : public StreamPlayer
{
    public:
        //! Default constructor. Sets up the computation.
        SpectrumRecorder(int windowSize = 1024, int sampleFrequency = -1, int maxHeight = 1024, int scale = 1);

        //! Destructor
        ~SpectrumRecorder();

        //! Sets up the processing
        void setup(int windowSize = 1024, int sampleFrequency = -1, int maxHeight = 1024, int scale = 1);

        //! Computes the spectrum
        bool compute(string file);

        //! Write the image (internal)
        void useBuffer();
        void sequenceEnds();

        //! Write down the image
        void writeToFile(string file);

    private:
        QImage mOut;
        SpectrumExtr mExtr;

        int mWindowSize;
        int mSampleFrequency;
        int mMaxHeight;
        int mScale;

        //boost::mutex mSwitchLock;
        int mCurrentRow;
        bool mComputed;
};

#endif
