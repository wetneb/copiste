#include "gui/view2D.h"

#include <iostream>
#include <QPaintEvent>
#include <QPainter>

View2D::View2D(QWidget *parent) : QWidget(parent)
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
}

QColor colorFromValue(neural_value value)
{
    if(value < 0)
        return QColor(127*(1.0+value),63,0);
    return QColor(63, 127*(1.0-value), 0);
}

void View2D::renderToImage(string fileName, string format, int w, int h)
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
        vector<float> inputVec;
        inputVec.resize(2);

        // Bulk draw : each pixel is computed exactly.
        // TODO : compute less points and interpolate between them.
        for(int x = 0; x != width(); ++x)
        {
            inputVec[0] = x*mViewport.scaleX + mViewport.x;
            for(int y = 0; y != height(); ++y)
            {
                inputVec[1] = y*mViewport.scaleY + mViewport.y;

                mNet->clean();
                float value = mNet->compute(inputVec);
                painter.setPen(colorFromValue(value));

                painter.drawPoint(x,y);
            }
        }
    }

    // And then draw the corpus
    if(mCorpus)
    {
        for(unsigned int index = 0; index != mCorpus->size(); ++index)
        {
            QPixmap *img = &mPoint0;
            if(mCorpus->elem(index)[0] == 1)
                img = &mPoint1;

            //painter.drawPoint((mCorpus->elem(index)[1] - mViewport.x)/mViewport.scaleX, (mCorpus->elem(index)[2]- mViewport.y)/mViewport.scaleY);
            painter.drawPixmap((mCorpus->elem(index)[1] - mViewport.x)/mViewport.scaleX - 5, (mCorpus->elem(index)[2]- mViewport.y)/mViewport.scaleY - 5, *img);
        }
    }

    painter.end();

    mViewport.img = newPic;

    // Backs up the rendered image
    mZoomHistory.push(mViewport);

    // Tells the user rendering ended
    emit rendered();
}

void View2D::setCorpus(Corpus *corpus)
{
    if(corpus->dimension() == 2)
    {
        mCorpus = corpus;
        vector<float> bounds = corpus->bounds();
        mViewport.scaleX = 1.8*(bounds[1] - bounds[0])/height();
        mViewport.scaleY = 1.8*(bounds[3] - bounds[2])/width();
        mViewport.x = bounds[0] - 0.1*(bounds[1] - bounds[0]);
        mViewport.y = bounds[2] - 0.1*(bounds[3] - bounds[2]);

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
    painter.end();
}

void View2D::mouseMoveEvent(QMouseEvent *event)
{
    if(mZooming)
    {
        int x = event->x(), y = event->y();
        QRect rect(min(x, mStartX), min(y, mStartY), max(x, mStartX), max(y, mStartY));

    }
}

void View2D::mousePressEvent(QMouseEvent *event)
{
    mStartX = event->x();
    mStartY = event->y();
    mZooming = true;
}

void View2D::mouseReleaseEvent(QMouseEvent *event)
{
    int x = event->x(), y = event->y();
    if(event->button() == Qt::LeftButton)
    {
        QRect rect(QPoint(min(x, mStartX), min(y, mStartY)), QPoint(max(x, mStartX), max(y, mStartY)));
        mZooming = false;

        if(rect.height() > 50 && rect.width() > 50)
        {
            Viewport newViewport = mViewport;
            newViewport.x += newViewport.scaleX*rect.x();
            newViewport.y += newViewport.scaleY*rect.y();
            newViewport.scaleX *= rect.width() / (float)width();
            newViewport.scaleY *= rect.height() / (float)height();

            mViewport = newViewport;

            renderScene();
            repaint();
        }
        else event->ignore();
    }
    else if(event->button() == Qt::RightButton)
    {
        neural_value *sampleVec = new neural_value[3]; // deleted by mCorpus->erase()
        sampleVec[0] = 1;
        sampleVec[1] = x*mViewport.scaleX + mViewport.x;
        sampleVec[2] = y*mViewport.scaleY + mViewport.y;
        mCorpus->addElem(sampleVec);
    }
    else event->ignore();
}

void View2D::handleKeyReleaseEvent(QKeyEvent *event)
{
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

void View2D::setNet(NNetwork *net)
{
    mNet = net;
}

#define MAX_WIDTH_HISTORY 2000

void plotHistory(float* history, int size, int corpusSize)
{
    QImage out(MAX_WIDTH_HISTORY,250,QImage::Format_RGB32);
    QPainter painter(&out);
    painter.fillRect(0,0,MAX_WIDTH_HISTORY,250,Qt::white);
    painter.setPen(QColor(0,0,0));
    float lastPoint = 1;

    for(int i = 0; i < size/corpusSize && i != MAX_WIDTH_HISTORY; ++i)
    {
        float newVal = 0;
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

NNetwork* View2D::net()
{
    return mNet;
}
