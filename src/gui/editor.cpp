#include "gui/editor.h"

Editor::Editor(QWidget *parent) : QMainWindow(parent)
{
    setCentralWidget(&mView);
    statusBar()->showMessage("Ready.");
    menuBar()->addAction("Redraw");
    menuBar()->addAction("Train");

    connect(&mView, SIGNAL(rendering()), this, SLOT(dispRendering()));
    connect(&mView, SIGNAL(rendered()), this, SLOT(dispRendered()));
    connect(menuBar(), SIGNAL(triggered(QAction*)), this, SLOT(handleAction(QAction*)));
}

void Editor::setNet(NNetwork *net)
{
    mView.setNet(net);
}

void Editor::setCorpus(Corpus *corpus)
{
    mView.setCorpus(corpus);
}

void Editor::handleAction(QAction *action)
{
    if(action->text() == "Redraw")
    {
        mView.renderScene();
        mView.repaint();
    }
    else if(action->text() == "Train")
    {
        mView.net()->randomize();
        mView.corpus()->train(*mView.net(), 0.01, 10000);
        mView.renderScene();
        mView.repaint();
    }
}

void Editor::keyReleaseEvent(QKeyEvent *event)
{
    mView.handleKeyReleaseEvent(event);
}

void Editor::dispRendering()
{
    statusBar()->showMessage("Rendering...");
}

void Editor::dispRendered()
{
    statusBar()->showMessage("Ready.");
}
