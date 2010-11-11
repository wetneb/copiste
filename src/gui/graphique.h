#ifndef INCLUDEDGRAPHIQUEH
#define INCLUDEDGRAPHIQUEH

#include <QPainter>
#include <QWidget>
#include <QTimer>
#include <QMutex>
#include <QTime>
#include <vector>
#include <iostream>
#include <cmath>
#include <vlc/vlc.h>

#define GRAPHIQUE_DATA_MAX_SIZE 5000
#define GRAPHIQUE_PAS 1
#define GRAPHIQUE_ECHELLE 50 // pixels par seconde

/**
 * \class Graphique
 * \brief Draws waveform profile or frenquency spectrum of the stream
 */
class Graphique : public QWidget
{
    Q_OBJECT
    public:
        //! Default constructor. Initializes everything.
        Graphique(QWidget *parent = 0);

        //! Redraws the diagram.
        void paintEvent(QPaintEvent *);

        //! Adds new data to the chart
        void appendData(uint16_t *data, int size);
        //! Adds new data to the chart
        void appendData(std::vector<int> data);
        //! Sets the data of the chart
        void setData(uint16_t *data, int size);
        //! Sets the data of the chart
        void setData(std::vector<int> data);
        //! Removes unused data
        void cleanData();

        //! Appends test data at the end of the stream (useful in a debugging process)
        void appendDummyData();

        //! Sets the draw mode of the chart (bars or lines)
        void setDrawBars(bool drawBars) { mDrawBars = drawBars; }

    public slots:
        //! Updates the chart when time is elapsed.
        void updateScrolling();

    private:
        QPainter mPainter;
        QTimer mRefreshTimer;

        int mFrequence;
        std::vector<int> mData;
        bool mDrawBars;

        // Scrolling
        int mScroll;
        QTime mTime;
        QMutex mScrollMutex;
};

#endif
