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


#include "gui/view2D.h"

#include <iostream>
#include <QPaintEvent>
#include <QPainter>

View2D::View2D(QWidget *parent) : QWidget(parent), mCurrentPoint(false)
{
    mViewport.x = 0;
    mViewport.y = 0;
    mViewport.scaleX = 1;
    mViewport.scaleY = 1;
    setFixedSize(800,600);
    mZooming = false;
    mNet = 0;
    mCorpus = 0;
    setWindowTitle("Neural network analysis tool");
    setMouseTracking(true);
}

int between0and255(int val)
{
    return std::min(255, std::max(0, val));
}

QColor colorFromValue(double value)
{
    if(value < 0.5)
        return QColor(between0and255(255*value),63,0);
    return QColor(63, between0and255(255*(1.0-value)), 0);
}

void View2D::renderToImage(std::string fileName, std::string format, int w, int h)
{
    QImage image(w, h, QImage::Format_RGB32);

    renderScene();

    image = mViewport.img.toImage();

    image.save(fileName.c_str(), format.c_str());
}

void View2D::renderScene()
{
    // Tells the user the rendering starts
    emit rendering();

    QPainter painter;
    QPixmap newPic(QSize(width(),height()));
    painter.begin(&newPic);

    // Load point images
    if(mPoint0.isNull())
        mPoint0.load(POINT_0_PATH);
    if(mPoint1.isNull())
        mPoint1.load(POINT_1_PATH);

    // Draw the network first
    if(mNet)
    {
        std::vector<double> inputVec;
        inputVec.resize(2);

        // Bulk draw : each pixel is computed exactly.
        //! \TODO : compute less points and interpolate between them.
        for(int x = 0; x != width(); ++x)
        {
            inputVec[0] = x*mViewport.scaleX + mViewport.x;
            for(int y = 0; y != height(); ++y)
            {
                inputVec[1] = y*mViewport.scaleY + mViewport.y;
                double value = mNet->classify(inputVec);
                painter.setPen(colorFromValue(value));

                painter.drawPoint(x,y);
            }
        }
    }
    else
        painter.fillRect(0, 0, width(), height(), QColor(127,127,127));

    painter.end();

    mViewport.img = newPic;

    // Backs up the rendered image
    mZoomHistory.push(mViewport);

    // Tells the user rendering ended
    emit rendered();
}

void View2D::setCorpus(Corpus *corpus)
{
    if(corpus->dimension() == 2 || corpus->size() != 0)
    {
        mCorpus = corpus;
        std::vector<double> bounds = corpus->bounds();
        double sizeX = (bounds[1] - bounds[0]), sizeY = (bounds[3] - bounds[2]);
        if(sizeX < CORPUS_EPSILON)
            sizeX = 1;
        if(sizeY < CORPUS_EPSILON)
            sizeY = 1;

        mViewport.scaleX = 1.2*sizeX/width();
        mViewport.scaleY = 1.2*sizeY/height();
        mViewport.x = bounds[0] - 0.1*sizeX;
        mViewport.y = bounds[2] - 0.1*sizeY;

        renderScene();
        repaint();
    }
    else std::cerr << "Unable to draw a corpus of dimension "<<corpus->dimension()<<", sorry.\n";
}

void View2D::paintEvent(QPaintEvent *event)
{
    // Displays the image onscreen
    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(0,0,mViewport.img);

    if(mZooming)
    {
        painter.setPen(QColor(0,0,0));
        int minX = std::min(mCurrentX, mStartX), minY = std::min(mCurrentY, mStartY), maxX = std::max(mCurrentX, mStartX), maxY = std::max(mCurrentY, mStartY);
        painter.drawRect(minX, minY, maxX - minX, maxY - minY);
    }

    QPixmap *img = &mPoint0;
    if(mCurrentPoint)
        img = &mPoint1;

    if(!mZooming && mCorpus)
        painter.drawPixmap(mCurrentX - img->width() / 2, mCurrentY - img->height() / 2, *img);

    // And then draw the corpus
    if(mCorpus)
    {
        for(unsigned int index = 0; index != mCorpus->size(); ++index)
        {
            QPixmap *img = &mPoint0;
            if(mCorpus->elem(index)[0] == 1)
                img = &mPoint1;

            painter.drawPixmap((mCorpus->elem(index)[1] - mViewport.x)/mViewport.scaleX - 5, (mCorpus->elem(index)[2]- mViewport.y)/mViewport.scaleY - 5, *img);
        }
    }

    painter.end();
}

void View2D::mouseMoveEvent(QMouseEvent *event)
{
    mCurrentX = event->x();
    mCurrentY = event->y();
    repaint();
}

void View2D::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        mStartX = event->x();
        mStartY = event->y();
        mCurrentX = mStartX;
        mCurrentY = mStartY;
        mZooming = true;
    }
}

void View2D::mouseReleaseEvent(QMouseEvent *event)
{
    int x = event->x(), y = event->y();
    if(event->button() == Qt::LeftButton)
    {
        QRect rect(QPoint(std::min(x, mStartX), std::min(y, mStartY)), QPoint(std::max(x, mStartX), std::max(y, mStartY)));
        mZooming = false;

        if(rect.height() > 50 && rect.width() > 50)
        {
            Viewport newViewport = mViewport;
            newViewport.x += newViewport.scaleX*rect.x();
            newViewport.y += newViewport.scaleY*rect.y();
            newViewport.scaleX *= rect.width() / (double)width();
            newViewport.scaleY *= rect.height() / (double)height();

            mViewport = newViewport;

            renderScene();
            repaint();
        }
        else event->ignore();
    }
    else if(event->button() == Qt::RightButton)
    {
        double *sampleVec = new double[3]; // deleted by mCorpus->erase()
        sampleVec[0] = mCurrentPoint;
        sampleVec[1] = x*mViewport.scaleX + mViewport.x;
        sampleVec[2] = y*mViewport.scaleY + mViewport.y;
        mCorpus->addElem(sampleVec);
    }
    else event->ignore();
}

void View2D::handleKeyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Minus)
    {
        Viewport newViewport = mViewport;
        newViewport.x -= mViewport.scaleX*width();
        newViewport.y -= mViewport.scaleY*height();
        newViewport.scaleX *= 3;
        newViewport.scaleY *= 3;

        mViewport = newViewport;

        renderScene();
        repaint();
    }
    else
        keyReleaseEvent(event);
}

void View2D::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Backspace)
    {
        mZoomHistory.pop();
        mViewport = mZoomHistory.top();

        repaint();
    }
    else if(event->key() == Qt::Key_R)
    {
        renderScene();
    }
    else event->ignore();
}

void View2D::setNet(NeuralNetwork *net)
{
    mNet = net;
}

#define MAX_WIDTH_HISTORY 2000

void plotHistory(double* history, int size, int corpusSize)
{
    QImage out(MAX_WIDTH_HISTORY,250,QImage::Format_RGB32);
    QPainter painter(&out);
    painter.fillRect(0,0,MAX_WIDTH_HISTORY,250,Qt::white);
    painter.setPen(QColor(0,0,0));
    double lastPoint = 1;

    for(int i = 0; i < size/corpusSize && i != MAX_WIDTH_HISTORY; ++i)
    {
        double newVal = 0;
        for(int j = i*corpusSize; j < i*corpusSize+corpusSize; ++j)
            newVal += history[j];
        newVal /= corpusSize;

        painter.drawLine(i-1,lastPoint*100,i,newVal*100);
        lastPoint = newVal;
    }
    painter.end();
    out.save("history.png", "PNG");
}

Corpus* View2D::corpus()
{
    return mCorpus;
}

NeuralNetwork* View2D::net()
{
    return mNet;
}

void View2D::setCurrentPoint(bool point)
{
    mCurrentPoint = point;
}
