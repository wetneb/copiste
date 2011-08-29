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


#ifndef INCLUDEDAUDIOCHUNKH
#define INCLUDEDAUDIOCHUNKH

/**
 * \class AudioChunk
 * \brief Stores PCM data of a sound sequence and computes some transformations on it
 */
class AudioChunk
{
    public:
        AudioChunk() : mData(0), size(0) { }

        void setData(uint8_t* data, unsigned int size, uint8_t bytesPerSample, uint8_t channels, unsigned int rate, int64_t pts);

        // AudioChunk operator+(const AudioChunk &audioChunk);

        int getValueByIndex(unsigned int index);
        int getValueByTime(int64_t time);

        AudioChunk* average(unsigned int passes);
        AudioChunk* reduceToBounds(unsigned int passes);

        unsigned int* computeSpectrum(int size);

    private:
        uint16_t* mData;
        unsigned int size;
};

#endif
