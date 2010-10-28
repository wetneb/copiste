#include "graphique.h"


Graphique::Graphique(QWidget *parent) : QWidget(parent),
                                 mPainter(this),
                                 mFrequence(50),
                                 mDrawBars(false)
{
    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(policy);
    setMinimumSize(QSize(100, 50));

    connect(&mRefreshTimer, SIGNAL(timeout()), this, SLOT(repaint()));
    connect(&mRefreshTimer, SIGNAL(timeout()), this, SLOT(updateScrolling()));
    mRefreshTimer.start(100);
    appendDummyData();

    mTime.start();
}

void Graphique::paintEvent(QPaintEvent *)
{
    mScrollMutex.lock();
    mPainter.begin(this);
    mPainter.fillRect(QRect(0, 0, size().width(), size().height()), QColor(0,0,0,255));
    mPainter.setPen(Qt::green);
    mPainter.setRenderHint(QPainter::Antialiasing);
    mScroll = 0;

    if(!mDrawBars)
    {
        QPoint lastPoint(size().width()-1, size().height()/2);
        QPoint newPoint;
        for(int i = 0; i <= size().width() / GRAPHIQUE_PAS; i++)
        {
            if((i*1000.0)/mFrequence > mScroll  && i - mScroll*mFrequence/1000.0 < mData.size())
                newPoint.setY((mData.at(mData.size() - i + mScroll*mFrequence/100000.0)*0.001+ 0)*size().height());
            else
                newPoint.setY(size().height()/2);

            newPoint.setX(lastPoint.x() - GRAPHIQUE_PAS);

            mPainter.drawLine(lastPoint, newPoint);
            //mPainter.fillRect(newPoint.x(), size().height()/2 - newPoint.y()/2, 2, newPoint.y(), Qt::green);
            lastPoint = newPoint;
        }
    }
    else
    {
        for(unsigned int i = 0; i != mData.size(); ++i)
        {
            mPainter.fillRect(size().width()*i/mData.size(), size().height() - mData[i]/400, size().width()/mData.size(), mData[i]/400, Qt::green);
        }
    }

    mPainter.end();
    mScrollMutex.unlock();
}

void Graphique::appendData(uint16_t *data, int size)
{
    for(int i = 0; i != size; i++)
        mData.push_back(data[i]);

    // cleanData();;
    mScrollMutex.lock();
    mScroll -= size/mFrequence*GRAPHIQUE_ECHELLE;
    mScrollMutex.unlock();
}

void Graphique::appendData(std::vector<int> data)
{
    for(unsigned int i = 0; i != data.size(); i++)
        mData.push_back(data[i]);

    // cleanData();

    mScrollMutex.lock();
    mScroll -= data.size()/mFrequence*GRAPHIQUE_ECHELLE;
    mScrollMutex.unlock();
}

void Graphique::cleanData()
{
    if(mData.size() >= 2*GRAPHIQUE_DATA_MAX_SIZE)
    {
        std::vector<int>::iterator fin = mData.begin();
        for(int i = 0; i != GRAPHIQUE_DATA_MAX_SIZE; i++)
            fin++;
        mData.erase(mData.begin(), fin);
    }
}

void Graphique::setData(uint16_t *data, int size)
{
    mData.clear();
    appendData(data, size);
}

void Graphique::setData(std::vector<int> data)
{
    mData.clear();
    appendData(data);
}

void Graphique::appendDummyData()
{
    for(int i = 0; i != 250; i++)
    {
        mData.push_back(sin((i/10.0)*2*acos(-1))*100);
    }
}

void Graphique::updateScrolling()
{
    mScrollMutex.lock();
    mScroll += mTime.elapsed();
    mScrollMutex.unlock();

    mTime.restart();
}
