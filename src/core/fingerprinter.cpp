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
Fingerprinter::Fingerprinter(bool verbose) : StreamPlayer(), mVerbose(verbose),
                                             mConsumer(0), mFpRead(0), mStarted(false)
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
    char *fp;
    chromaprint_finish(mCtxt);
    chromaprint_get_fingerprint(mCtxt, &fp);
    int new_size = strlen(fp);
    if(new_size > 0)
    {
        std::cout << "got size "<<new_size<< std::endl;
        for(int i = mFpRead; i < new_size; i++)
            std::cout << fp[i];
    }
    chromaprint_dealloc(fp);

}

//! Computes the fingerprint
void Fingerprinter::useBuffer()
{
    if(!mStarted)
    {
        cout << "freq : "<<mFrequency<<std::endl;
        chromaprint_start(mCtxt, mFrequency, 1);
        mStarted = true;
    }

    uint16_t* buf = new uint16_t[chunkSize()];
    for(int i = 0; i < chunkSize(); i++)
        buf[i] = mBuffer[i];

    chromaprint_feed(mCtxt, buf, chunkSize());
    delete buf;

}


