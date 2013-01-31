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


#ifndef INCLUDED_LIVEPLAYERH
#define INCLUDED_LIVEPLAYERH

#include <QWidget>
#include <QKeyEvent>
#include <QImage>
#include <boost/thread.hpp>
#include "core/featuredrawer.h"
#include "algo/neuralnetwork.h"

const int LIVE_PLAYER_HEIGHT = 600;
const int LIVE_PLAYER_WIDTH = 800;

//! Main class for live stream classification and visualisation
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
        //! Key handling (volume)
        void keyReleaseEvent(QKeyEvent *event);

    private:
        QImage mLastImage;
        int *mLastPoints;
        boost::mutex mLock;
        libvlc_time_t mLastUpdate;
};

#endif

