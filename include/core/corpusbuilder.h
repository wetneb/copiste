#ifndef INCLUDED_FILEANALYSERH
#define INCLUDED_FILEANALYSERH

// Needed to read directories
#include <boost/filesystem.hpp>

#include "soundanalyser.h"

//! Creates a corpus from a set of audio files
class CorpusBuilder : private SoundAnalyser
{
    public:
        //! Constructor
        CorpusBuilder();

        //! Set up from an XML file
        bool setup(string fileName);
        //! Write down the results
        bool write(Corpus* corpus);

        //! Compute
        void compute();

        //! Sets verbosity
        void setVerbose(bool verbose) { mVerbose = verbose; }

        //! Mirror from SoundAnalyser
        bool setupPipeline(string filename) { return ((SoundAnalyser*)this)->setupPipeline(filename); }

    private:
        string mBasePath;
        vector<string> mFiles;
        vector<bool> mGoals;
        vector<float*> mResults;
        int mCurrentFile;
        bool mVerbose;
};

#endif
