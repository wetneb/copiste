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


#ifndef INCLUDED_VIEW2DH
#define INCLUDED_VIEW2DH

class View2D;

#include "algo/neuralnetwork.h"
#include "algo/corpus.h"
#include "gui/viewport.h"

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QTime>

#include <iostream>
#include <stack>

#define VIEW_OUTPUT_WIDTH 700
#define VIEW_OUTPUT_HEIGHT 500
#define POINT_0_PATH "img/point-0.png"
#define POINT_1_PATH "img/point-1.png"
#define CORPUS_EPSILON 0.0001
#define MAX_WIDTH_HISTORY 2000
#define TIME_BETWEEN_TWO_LABEL_UPDATES 500 // in milliseconds

void plotHistory(double* history, int size, int corpusSize);

//! A renderer for a 2D neural network
class View2D : public QWidget
{
    Q_OBJECT
    public:
        //! Constructor. Sets up the renderer.
        View2D(QWidget *parent = 0);

        //! Render the graph to a file rather than displaying it on the screen
        void renderToImage(std::string fileName, std::string format = "PNG",
                int w = VIEW_OUTPUT_WIDTH, int h = VIEW_OUTPUT_HEIGHT);

        //! Sets the corpus to render
        void setCorpus(Corpus *corpus);
        //! Sets the network to render
        void setNet(NeuralNetwork *net);
        //! Updates the current quadtree
        void updateQuadtree();

        //! Rendering function.
        void renderScene();

        //! Returns the corpus
        Corpus* corpus();
        //! Returns the network
        NeuralNetwork* net();

        //! Sends key events from parent widgets
        void handleKeyReleaseEvent(QKeyEvent *event);

        //! Change the type of point
        void setCurrentPoint(bool point);

    signals:
        //! Rendering started (signal)
        void rendering();
        //! Rendering finished (signal)
        void rendered();
        //! User hovered an element (signal)
        void hoveringElement(std::string);

    protected:
        //! Event handling function (redraws the scene)
        void paintEvent(QPaintEvent *event);

        //! Handles dynamic zoom
        void mouseMoveEvent(QMouseEvent *event);
        //! Handles dynamic zoom
        void mousePressEvent(QMouseEvent *event);
        //! Handles dynamic zoom
        void mouseReleaseEvent(QMouseEvent *event);
        //! Handles dynamic zoom
        void keyReleaseEvent(QKeyEvent *event);

        //! Emits the signal hoveringElement(std::string)
        void emitHoveringElement(std::string name);

    private:
        Corpus *mCorpus;
        NeuralNetwork *mNet;

        QPixmap mPoint0;
        QPixmap mPoint1;

        // Viewport managing
        Viewport mViewport;

        //! Last update for corpus labels
        QTime mLastUpdate;

        // Dynamic zoom
        bool mZooming;
        int mStartX;
        int mStartY;
        int mCurrentX;
        int mCurrentY;

        // Corpus editing
        bool mCurrentPoint;

        // Zoom history
        std::stack<Viewport> mZoomHistory;
};

#endif

