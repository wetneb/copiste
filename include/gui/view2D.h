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

#include <QWidget>
#include <stack>
#include "algo/neuralnetwork.h"
#include "algo/corpus.h"

#define VIEW_OUTPUT_WIDTH 700
#define VIEW_OUTPUT_HEIGHT 500
#define POINT_0_PATH "img/point-0.png"
#define POINT_1_PATH "img/point-1.png"
#define CORPUS_EPSILON 0.0001

/** \todo Include a BSP tree to display infoboxes when hovering points (with the labels of the corpus) */

//! Internal struture used by View2D to keep in memory old views of a same network
struct Viewport
{
    double x;
    double y;
    double scaleX;
    double scaleY;
    QPixmap img;
};
typedef struct Viewport Viewport;

void plotHistory(double* history, int size, int corpusSize);

//! A renderer for a 2D neural network
class View2D : public QWidget
{
    Q_OBJECT
    public:
        //! Constructor. Sets up the renderer.
        View2D(QWidget *parent = 0);

        //! Render the graph to a file rather than displaying it on the screen
        void renderToImage(std::string fileName, std::string format = "PNG", int w = VIEW_OUTPUT_WIDTH, int h = VIEW_OUTPUT_HEIGHT);

        //! Sets the corpus to render
        void setCorpus(Corpus *corpus);
        //! Sets the network to render
        void setNet(NeuralNetwork *net);

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
        void rendering();
        void rendered();

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

    private:
        Corpus *mCorpus;
        NeuralNetwork *mNet;

        QPixmap mPoint0;
        QPixmap mPoint1;

        // Viewport managing
        Viewport mViewport;

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
