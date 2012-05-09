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


#include "core/liveplayer.h"

LivePlayer::LivePlayer(QWidget *parent) : QWidget(parent),
                                             FeatureDrawer(true),
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
    painter.setPen(QColor(255,255,255));
    painter.drawText(0,0, width(), height(), Qt::AlignCenter, "Patience... on se connecte...");
    painter.end();

    repaint();
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
