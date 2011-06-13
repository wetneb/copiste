#ifndef INCLUDED_LIVEPLAYERH
#define INCLUDED_LIVEPLAYERH

#include <QWidget>
#include <QImage>
#include <boost/thread.hpp>
#include "core/featuredrawer.h"
#include "algo/nnetwork.h"

#define LIVE_PLAYER_HEIGHT 600
#define LIVE_PLAYER_WIDTH 800



class LivePlayer : public QWidget, public FeatureDrawer
{
    public:
        //! Constructor
        LivePlayer(QWidget *parent = 0);

        //! Updates the view
        void useFeatures();

    protected:
        //! Event handling function (redraws the scene)
        void paintEvent(QPaintEvent *event);

    private:
        NNetwork *mNet;

        QImage mLastImage;
        int *mLastPoints;
        boost::mutex mLock;
        libvlc_time_t mLastUpdate;
};

#endif
