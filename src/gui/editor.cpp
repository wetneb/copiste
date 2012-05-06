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


#include "gui/editor.h"

Editor::Editor(QWidget *parent) : QMainWindow(parent)
{
    setCentralWidget(&mView);
    statusBar()->showMessage("Ready.");
    menuBar()->addAction("Redraw");
    menuBar()->addAction("Train");
    menuBar()->addAction("Reset");
    menuBar()->addAction("Save network");
    menuBar()->addAction("Save corpus");

    mToolbar = addToolBar("Corpus Tools");
    mToolbar->setAllowedAreas(Qt::LeftToolBarArea);
    mToolbar->addAction(QIcon(POINT_0_PATH),"Black point");
    mToolbar->addAction(QIcon(POINT_1_PATH),"White point");

    connect(&mView, SIGNAL(rendering()), this, SLOT(dispRendering()));
    connect(&mView, SIGNAL(rendered()), this, SLOT(dispRendered()));
    connect(menuBar(), SIGNAL(triggered(QAction*)), this, SLOT(handleAction(QAction*)));
    connect(mToolbar, SIGNAL(actionTriggered(QAction*)), this, SLOT(handleAction(QAction*)));
}

void Editor::setNet(NeuralNetwork *net)
{
    mView.setNet(net);
}

void Editor::setCorpus(Corpus *corpus)
{
    mView.setCorpus(corpus);
}

void Editor::setRegularization(float r)
{
    mRegularization = r;
}

void Editor::setTrainingRate(float r)
{
    mTrainingRate = r;
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
        if(mView.net())
            mView.net()->randomize();
    }
    else if(action->text() == "Train")
    {
        if(mView.corpus() && mView.net())
        {
            std::cout << "Training ended with cost " << mView.net()->train(*mView.corpus(), mTrainingRate, mRegularization) << std::endl;
            mView.renderScene();
            mView.repaint();
        }
        else if(mView.corpus() == 0)
            statusBar()->showMessage("There's no suitable corpus to train the network.");
        else statusBar()->showMessage("There's no suitable network to be trained.");
    }
    else if(action->text() == "Focus")
    {
        mView.setFocus(Qt::OtherFocusReason);
    }
    else if(action->text() == "Black point")
    {
        mView.setCurrentPoint(false);
    }
    else if(action->text() == "White point")
    {
        mView.setCurrentPoint(true);
    }
    else if(action->text() == "Save corpus")
    {
        if(mView.corpus())
        {
            mView.corpus()->write(
            QFileDialog::getOpenFileName(this,
                "Save corpus", "~", tr("XML files (*.xml)")).toStdString());
        }
    }
    else if(action->text() == "Save network")
    {
        if(mView.net())
        {
            mView.net()->toFile(
            QFileDialog::getOpenFileName(this,
                "Save network", "~", tr("XML files (*.xml)")).toStdString());
        }
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
