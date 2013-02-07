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


#include "core/fingerprinter.h"

class FingerprintConsumer;

//! Constructor
Fingerprinter::Fingerprinter(bool verbose, bool live) :
   StreamPlayer(live), mVerbose(verbose),
   mConsumer(0), mFpRead(0), mChunksFed(0)
{
    mCtxt = chromaprint_new(CHROMAPRINT_ALGORITHM_DEFAULT);
}

Fingerprinter::~Fingerprinter()
{
    chromaprint_free(mCtxt);
}

//! Sets the consumer to call when a fingerprint is produced
void Fingerprinter::addConsumer(FingerprintConsumer* consumer)
{
    mConsumer.push_back(consumer);
}

//! Unregister a consumer
void Fingerprinter::remConsumer(FingerprintConsumer* consumer)
{
    for(std::vector<FingerprintConsumer*>::iterator iter = mConsumer.begin();
            iter != mConsumer.end(); iter++)
    {
        if(*iter == consumer)
            mConsumer.erase(iter);
        iter = mConsumer.end();
    }
}

void Fingerprinter::sequenceEnds()
{
    ;
}

//! Sends a fingerprint to a consumer (if any)
void Fingerprinter::sendFp(fingerp fp)
{
    for(std::vector<FingerprintConsumer*>::iterator iter = mConsumer.begin();
            iter != mConsumer.end(); iter++)
    {
        (*iter)->consumeFingerprint(fp);
    }
}

//! Computes the fingerprint
void Fingerprinter::useBuffer()
{
    if(mChunksFed == 0)
    {   
        chromaprint_start(mCtxt, mFrequency, mChannels);
    }

    chromaprint_feed(mCtxt, mBuffer, mChannels*chunkSize());
    mChunksFed++;

    if(mChunksFed == 32)
    {
        mChunksFed = 0;
        chromaprint_finish(mCtxt);
        int32_t *fp;
        int size;
        chromaprint_get_raw_fingerprint(mCtxt, (void**)&fp, &size);
        for(int i = 0; i < size; i++)
        {
            if(mVerbose)
                cout << fp[i] << " ";
            sendFp(fp[i]);
        }
        if(mVerbose)
            cout << endl;
        chromaprint_dealloc(fp);
    }
}


