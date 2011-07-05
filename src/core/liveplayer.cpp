#include "core/liveplayer.h"

LivePlayer::LivePlayer(QWidget *parent) : QWidget(parent),
                                             FeatureDrawer(true),
                                             mNet(0),
                                             mLastImage(LIVE_PLAYER_WIDTH, LIVE_PLAYER_HEIGHT, QImage::Format_RGB32),
                                             mLastPoints(0),
                                             mLastUpdate(0)
{
    setFixedSize(LIVE_PLAYER_WIDTH, LIVE_PLAYER_HEIGHT);
    setImageSize(LIVE_PLAYER_WIDTH, LIVE_PLAYER_HEIGHT);
    setWindowTitle("Live stream classification - Copiste");

    QPainter painter(&mLastImage);
    painter.fillRect(0,0, width(), height(), QColor(0,0,0));
    painter.setFont(QFont("Arial", 25));
    painter.drawText(0,0, width(), height(), Qt::AlignCenter, "Loading...");

    update();
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
