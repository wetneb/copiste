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
    menuBar()->addAction("Settings");
    menuBar()->addAction("Reset");
    menuBar()->addAction("Save network");
    menuBar()->addAction("Save corpus");

    mToolbar = addToolBar("Corpus Tools");
    mToolbar->setAllowedAreas(Qt::LeftToolBarArea);
    mToolbar->addAction(QIcon(POINT_0_PATH),"Black point");
    mToolbar->addAction(QIcon(POINT_1_PATH),"White point");

    setWindowTitle("Neural Network Analysis");

    connect(&mView, SIGNAL(rendering()), this, SLOT(dispRendering()));
    connect(&mView, SIGNAL(rendered()), this, SLOT(dispRendered()));
    connect(menuBar(), SIGNAL(triggered(QAction*)), this, SLOT(handleRequest(QAction*)));
    connect(mToolbar, SIGNAL(actionTriggered(QAction*)), this, SLOT(handleRequest(QAction*)));
}

void Editor::setNet(NeuralNetwork *net)
{
    mView.setNet(net);
}

void Editor::setCorpus(Corpus *corpus)
{
    mView.setCorpus(corpus);
}

void Editor::setRegularization(double r)
{
    mRegularization = r;
}

void Editor::setTrainingRate(double r)
{
    mTrainingRate = r;
}

void Editor::setIter(int iter)
{
    mIter = iter;
}

void Editor::setDebug(bool debug)
{
    mDebug = debug;
}

void Editor::handleRequest(QAction *action)
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
            std::cout << "Training ended with cost " << mView.net()->train(*mView.corpus(), mTrainingRate, mRegularization, mIter, mDebug) << std::endl;
            mView.renderScene();
            mView.repaint();
        }
        else if(mView.corpus() == 0)
            statusBar()->showMessage("There's no suitable corpus to train the network.");
        else statusBar()->showMessage("There's no suitable network to be trained.");
    }
    else if(action->text() == "Settings")
    {
        SettingsDialog sd(mTrainingRate, mRegularization, mIter, mDebug);
        QObject::connect(&sd, SIGNAL(values(double, double, int, bool)), this, SLOT(updateSettings(double, double, int, bool)));
        sd.exec(); 
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

void Editor::updateSettings(double rate, double reg, int iter, bool debug)
{
    mTrainingRate = rate;
    mRegularization = reg;
    mIter = iter;
    mDebug = debug;
}

SettingsDialog::SettingsDialog(double rate, double reg, int iter, bool debug, QWidget *parent) : QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QHBoxLayout *rateLayout = new QHBoxLayout;
    rateLayout->addWidget(new QLabel("Learning rate :"));
    rateLayout->addWidget(&mRate);
    mRate.setRange(0,10);
    mRate.setDecimals(5);
    mRate.setValue(rate);
    mainLayout->addLayout(rateLayout);

    QHBoxLayout *regLayout = new QHBoxLayout;
    regLayout->addWidget(new QLabel("Regularization :"));
    regLayout->addWidget(&mReg);
    mReg.setRange(0,1000);
    mReg.setDecimals(5);
    mReg.setValue(reg);
    mainLayout->addLayout(regLayout);

    QHBoxLayout *iterLayout = new QHBoxLayout;
    iterLayout->addWidget(new QLabel("Iterations :"));
    iterLayout->addWidget(&mIter);
    mIter.setRange(1,10000000);
    mIter.setValue(iter);
    mainLayout->addLayout(iterLayout);

    QHBoxLayout *debugLayout = new QHBoxLayout;
    debugLayout->addWidget(new QLabel("Debugging :"));
    debugLayout->addWidget(&mDebug);
    mDebug.setChecked(debug);
    mainLayout->addLayout(debugLayout);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    QPushButton *ok = new QPushButton("OK"), *cancel = new QPushButton("Cancel");
    buttonsLayout->addWidget(ok);
    buttonsLayout->addWidget(cancel);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);

    QObject::connect(cancel, SIGNAL(released()), this, SLOT(close()));
    QObject::connect(ok, SIGNAL(released()), this, SLOT(emitValues()));
    QObject::connect(ok, SIGNAL(released()), this, SLOT(close()));
}

void SettingsDialog::emitValues()
{
    emit values(mRate.value(), mReg.value(), mIter.value(), mDebug.isChecked());
}
