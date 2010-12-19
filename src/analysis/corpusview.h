#ifndef INCLUDEDCORPUSVIEWH
#define INCLUDEDCORPUSVIEWH

#include <QImage>
#include <QPainter>
#include "algo/corpus.h"
#include "algo/nnetwork.h"

#define CORPUS_VIEW_OUTPUT_WIDTH 800
#define CORPUS_VIEW_OUTPUT_HEIGHT 600

class CorpusView
{
    public:
        CorpusView() : mImage(CORPUS_VIEW_OUTPUT_WIDTH, CORPUS_VIEW_OUTPUT_HEIGHT, QImage::Format_RGB32), mPainter(&mImage) { erase(); }

        void setViewPort(float scaleX, float scaleY, float origX = 0, float origY = 0);

        void erase();

        void redraw();

        void setCorpus(Corpus* corpus) { mCorpus = corpus; }

        void addNetworkImage(NNetwork* network) { mNetwork = network; }

        void addNetworkZeros();

        void write(string fileName, string format = "PNG");

        QImage image() { return mImage; }

    private:
        QImage mImage;
        QPainter mPainter;
        Corpus *mCorpus;
        NNetwork *mNetwork;

        float mScaleX;
        float mScaleY;
        float mOrigX;
        float mOrigY;

};


#endif
