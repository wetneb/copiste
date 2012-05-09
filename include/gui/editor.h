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

// Settings dialog
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class Editor;

#include "gui/view2D.h"
#include "algo/neuralnetwork.h"
#include "algo/corpus.h"

//! A settings dialog
class SettingsDialog : public QDialog
{
    Q_OBJECT
    public:
        SettingsDialog(double rate, double reg, int iter, QWidget *parent = 0);

    public slots:
        void emitValues();
    signals:
        void values(double rate, double reg, int iter);

    private:
        QDoubleSpinBox mRate, mReg;
        QSpinBox mIter;
};

//! The main class for the graphical network trainer
class Editor : public QMainWindow
{
    Q_OBJECT
    public:
        Editor(QWidget *parent = 0);

        void setNet(NeuralNetwork *net);
        void setCorpus(Corpus *corpus);

        void setRegularization(double r);
        void setTrainingRate(double r);
        void setIter(int iter);

    protected:
        void keyReleaseEvent(QKeyEvent *event);

    public slots:
        void dispRendering();
        void dispRendered();
        void handleRequest(QAction *action);
        void updateSettings(double rate, double reg, int iter);

    private:
        View2D mView;
        QToolBar *mToolbar;

        // Training parameters
        double mTrainingRate, mRegularization;
        int mIter;
};

#endif
