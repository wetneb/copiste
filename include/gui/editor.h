#ifndef INCLUDED_EDITORH
#define INCLUDED_EDITORH

#include <QMainWindow>
#include <QStatusBar>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>

class Editor;

#include "gui/view2D.h"
#include "algo/nnetwork.h"
#include "algo/corpus.h"

//! The main class for the graphical network trainer
class Editor : public QMainWindow
{
    Q_OBJECT
    public:
        Editor(QWidget *parent = 0);

        void setNet(NNetwork *net);
        void setCorpus(Corpus *corpus);

    protected:
        void keyReleaseEvent(QKeyEvent *event);

    public slots:
        void dispRendering();
        void dispRendered();
        void handleAction(QAction *action);

    private:
        View2D mView;
        QToolBar *mToolbar;
};

#endif
