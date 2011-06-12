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

        boost::mutex mSwitchLock;
        int mCurrentRow;
        bool mComputed;
};

#endif
