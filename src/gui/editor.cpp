#include "gui/editor.h"

Editor::Editor(QWidget *parent) : QMainWindow(parent)
{
    setCentralWidget(&mView);
    statusBar()->showMessage("Ready.");
    menuBar()->addAction("Redraw");
    menuBar()->addAction("Train");
    menuBar()->addAction("Reset");

    mToolbar = addToolBar("Corpus Tools");
    mToolbar->setAllowedAreas(Qt::LeftToolBarArea);
    mToolbar->addAction(QIcon(POINT_0_PATH),"Black point");
    mToolbar->addAction(QIcon(POINT_1_PATH),"White point");

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
    else if(action->text() == "Reset")
    {
        mView.net()->randomize();
    }
    else if(action->text() == "Train")
    {
        mView.corpus()->train(*mView.net(), 0.001, 10000);
        mView.renderScene();
        mView.repaint();
    }
    else if(action->text() == "Focus")
    {
        mView.setFocus(Qt::OtherFocusReason);
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
