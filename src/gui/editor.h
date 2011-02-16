#ifndef INCLUDED_EDITORH
#define INCLUDED_EDITORH

#include <QWidget>
#include "algo/nnetwork.h"
#include "algo/corpus.h"
#include "analysis/corpusview.h"

class Editor : public QWidget
{
    Q_OBJECT
    public:
        Editor(QWidget *parent = 0);

        void setCorpus(Corpus *corpus);
        void setNet(NNetwork *net) { mNet = net; }

    private:
        Corpus *mCorpus;
        NNetwork *mNet;
        CorpusView *mView;

        float mX;
        float mY;
        float mScaleX;
        float mScaleY;
};

#endif
