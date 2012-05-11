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
        vector<double*> mResults;
        vector<int> mNbElems;
        int mCurrentFile;
        bool mVerbose;
        int mElemLength;
        int mCompOffset;
};

#endif
