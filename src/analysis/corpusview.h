#ifndef INCLUDEDCORPUSVIEWH
#define INCLUDEDCORPUSVIEWH

#include <QImage>
#include <QPainter>
#include "algo/corpus.h"
#include "algo/nnetwork.h"

#define CORPUS_VIEW_OUTPUT_WIDTH 800
#define CORPUS_VIEW_OUTPUT_HEIGHT 600

#define POINT_0_PATH "img/point-0.png"
#define POINT_1_PATH "img/point-1.png"

class CorpusView
{
    public:
        CorpusView();

        void setViewPort(float scaleX, float scaleY, float origX = 0, float origY = 0);

        void erase();

        void redraw(bool verbose = false);

        void setCorpus(Corpus* corpus) { mCorpus = corpus; }

        void addNetworkImage(NNetwork* network) { mNetwork = network; }

        void addNetworkZeros();

        void write(string fileName, string format = "PNG", bool verbose = false);

        QImage image() { return mImage; }

    private:
        QImage mImage;
        QPainter mPainter;
        Corpus *mCorpus;
        NNetwork *mNetwork;

        QPixmap mPoint0;
        QPixmap mPoint1;

        float mScaleX;
        float mScaleY;
        float mOrigX;
        float mOrigY;

};


#endif
