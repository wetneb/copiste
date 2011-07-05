#ifndef INCLUDED_FILEANALYSERH
#define INCLUDED_FILEANALYSERH

class CorpusBuilder;

// Needed to read directories
#include <boost/filesystem.hpp>

#include "core/soundanalyser.h"

//! Creates a corpus from a set of audio files
class CorpusBuilder : private SoundAnalyser
{
    public:
        //! Constructor
        CorpusBuilder();

        //! Set what length (in seconds) should be used as training element length (0 : use the whole files)
        void setElementLength(int sec) { mElemLength = sec; }

        //! Set computing offset (the time at the beginning of each file we should forget)
        void setComputingOffset(int sec) { mCompOffset = sec; }

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
        vector<string> mSampleNames;
        vector<float*> mResults;
        vector<int> mNbElems;
        int mCurrentFile;
        bool mVerbose;
        int mElemLength;
        int mCompOffset;
};

#endif
