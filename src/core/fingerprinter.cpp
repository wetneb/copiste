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
Fingerprinter::Fingerprinter(bool verbose) : StreamPlayer(true), mVerbose(verbose),
                                             mConsumer(0), mFpRead(0), mChunksFed(0)
{
    mCtxt = chromaprint_new(CHROMAPRINT_ALGORITHM_DEFAULT);
}

Fingerprinter::~Fingerprinter()
{
    chromaprint_free(mCtxt);
}

//! Sets the consumer to call when a fingerprint is produced
void Fingerprinter::setConsumer(FingerprintConsumer* consumer)
{
    mConsumer = consumer;
}

void Fingerprinter::sequenceEnds()
{
    ;
}

//! Sends a fingerprint to a consumer (if any)
void Fingerprinter::sendFp(fingerp fp)
{
    if(mConsumer)
        mConsumer->consumeFingerprint(fp);
}

//! Computes the fingerprint
void Fingerprinter::useBuffer()
{
    if(mChunksFed == 0)
    {   
        chromaprint_start(mCtxt, mFrequency, 1);
    }

    int16_t* buf = new int16_t[chunkSize()];
    for(int i = 0; i < chunkSize(); i++)
        buf[i] = mBuffer[i];

    chromaprint_feed(mCtxt, buf, chunkSize());
    delete buf;
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
            cout << fp[i] << " ";
            sendFp(fp[i]);
        }
        cout << endl;
        chromaprint_dealloc(fp);
    }
}


