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


#ifndef INCLUDED_EDITORH
#define INCLUDED_EDITORH

#include <QMainWindow>
#include <QStatusBar>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>

class Editor;

#include "gui/view2D.h"
#include "algo/neuralnetwork.h"
#include "algo/corpus.h"

//! The main class for the graphical network trainer
class Editor : public QMainWindow
{
    Q_OBJECT
    public:
        Editor(QWidget *parent = 0);

        void setNet(NeuralNetwork *net);
        void setCorpus(Corpus *corpus);

        void setRegularization(float r);
        void setTrainingRate(float r);

    protected:
        void keyReleaseEvent(QKeyEvent *event);

    public slots:
        void dispRendering();
        void dispRendered();
        void handleAction(QAction *action);

    private:
        View2D mView;
        QToolBar *mToolbar;

        float mTrainingRate, mRegularization;
};

#endif
