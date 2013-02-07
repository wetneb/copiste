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


#ifndef INCLUDED_FINGERPRINTERH
#define INCLUDED_FINGERPRINTERH

#include "core/streamplayer.h"
#include <chromaprint.h>

#include <vector>

//! Type of a fingerprint
typedef int fingerp;

#include "core/fingerprintconsumer.h"

//! A class reading a file and producing fingerprints
class Fingerprinter : public StreamPlayer
{
    public:
        //! Constructor
        Fingerprinter(bool verbose = true, bool live = false);

        ~Fingerprinter();

        //! Add a consumer to call when a fingerprint is produced
        void addConsumer(FingerprintConsumer* consumer);
        //! Unregister a consumer
        void remConsumer(FingerprintConsumer* consumer);

        //! Reimplement the callback defined in StreamPlayer
        void useBuffer();
        void sequenceEnds();
    private:
        //! Send a fingerprint to the consumer (if any)
        void sendFp(fingerp fp);

        bool mVerbose;
        std::vector<FingerprintConsumer*> mConsumer;
        ChromaprintContext *mCtxt;
        int mFpRead;
        int mChunksFed;
};

#endif

