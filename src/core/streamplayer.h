#ifndef INCLUDEDSTREAMPLAYERH
#define INCLUDEDSTREAMPLAYERH

#include <vlc/vlc.h>
#include <QString>
#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QMessageBox>
#include <QFile>

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include "gui/graphique.h"
#include "algo/analysis.h"

// Temporaire, à supprimer
#include <string>

// Callbacks audio
void handleStream(void* p_audio_data, uint8_t* p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, unsigned int size, int64_t pts);
void prepareRender(void* p_audio_data, uint8_t** pp_pcm_buffer , unsigned int size);

/**
 * \class StreamPlayer
 * \brief Manages stream decoding and sends it to the computing part of the application
 */
class StreamPlayer : public QObject
{
    Q_OBJECT
    public:
        //! Default constructor. Starts VLC instance.
        StreamPlayer();
        //! Default destructor. Frees memory and closes VLC.
        ~StreamPlayer();

        //! Returns URL of the currently playing stream
        QString url() { return mUrl; }

        //! Audio Mutex (to prevent from crashes with VLC)
        QMutex mLock;

        //! Sets the wawe diagram
        void setGraphiqueOnde(Graphique *graphique) { mGraphique = graphique; }
        //! Set the spectrum diagram
        void setGraphiqueSpectre(Graphique *graphique) { mSpectre = graphique; }
        //! Returns the wawe diagram
        Graphique* graphiqueOnde() { return mGraphique; }
        //! Returns the spectrum diagram
        Graphique* graphiqueSpectre() { return mSpectre; }

        //! Converts an array of uint8_t to another array of uint16_t (assuming the values are coded on two bytes)
        static uint16_t* convert8to16(const uint8_t* source, int size);
        //! Computes the average value of the stream, on a given frequency
        static uint16_t* average(uint16_t* source, int size, int passes, int scale= 1);
        //! Reduces the stream, keeping only the extreme values
        static void reduce(uint16_t* source, uint16_t* dest, int size, int passes, int scale=1);
        //! Adds an offset to each value of the array
        static void addOffset(uint16_t* source, uint16_t* dest, int size, int offset);
        //! Dumps values to a file (useful in a debugging process) (16 bit version)
        void dumpStreamToFile16(uint16_t* source, int size);
        //! Dumps values to a file (8 bit version)
        void dumpStreamToFile8(uint8_t* source, int size);
        //! Write a line to the dump file
        void writeLine(std::string line);

    public slots:
        //! Starts playing the stream
        void play();
        //! Stop the currenty playing stream
        void stop();
        //! Changes the URL of the stream
        void setUrl(QString url) { mUrl = url; }
        //! Changes the volume of the stream
        void setVol(int val);

    private slots:
        void update();

    private:
        // Paramètres
        QString mUrl;
        bool mPlaying;

        // VLC
        libvlc_instance_t *mVlcInstance;
        libvlc_media_player_t *mMp;
        libvlc_media_t *mMedia;

        // Lecture
        QTimer mTimer;

        // Temporaire
        Graphique* mGraphique;
        Graphique* mSpectre;

        // Debug
        QFile mDumpFile;
        bool mDebugWritten;
};

#endif
