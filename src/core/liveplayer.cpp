#include "core/liveplayer.h"

LivePlayer::LivePlayer(QWidget *parent) : QWidget(parent),
                                             FeatureDrawer(true),
                                             mNet(0),
                                             mLastImage(LIVE_PLAYER_WIDTH, LIVE_PLAYER_HEIGHT, QImage::Format_RGB32),
                                             mLastPoints(0)
{
    setFixedSize(LIVE_PLAYER_WIDTH, LIVE_PLAYER_HEIGHT);
    setImageSize(LIVE_PLAYER_WIDTH, LIVE_PLAYER_HEIGHT);
    setWindowTitle("Live stream classification - Copiste");
}

void LivePlayer::useFeatures()
{
    if(mLastUpdate < getPlayingTime() - 500)
    {
        draw("Live stream", true);
        writeToDevice(&mLastImage);

        update();
        mLastUpdate = getPlayingTime();

        if(nbSamples() >= 4*LIVE_PLAYER_WIDTH)
            cleanOldFeatures(LIVE_PLAYER_WIDTH*1.1);
    }
}

void LivePlayer::paintEvent(QPaintEvent *event)
{
    // Displays the image onscreen
    QPainter painter;
    painter.begin(this);
    painter.drawImage(0,0,mLastImage);
    painter.end();
}
