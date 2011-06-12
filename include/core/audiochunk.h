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
