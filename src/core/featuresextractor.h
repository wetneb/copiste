#ifndef INCLUDED_FEATURESEXTRACTORH
#define INCLUDED_FEATURESEXTRACTORH

//! Main class used to extract data from audio files
class FeaturesExtractor;

#include "core/streamplayer.h"
// Uses boost : ideally, we should not depend from Qt when the application doesn't uses GUI features…
#include <boost/program_options.hpp>

class FeaturesExtractor
{
        //! Default constructor. If specified, sets everything up using command-line arguments
        FeaturesExtractor(int argc = 0, char** argv = 0);

        //! Compute the values
        void run();
    private:
        StreamPlayer mSp;

        float *mResults;
}

#endif
