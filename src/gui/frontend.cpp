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

#include "gui/frontend.h"

HMMFrontend::HMMFrontend(QWidget* parent) : QWidget(parent), mModel(0),
    mPlayer(0)
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(&mLabel);
    layout->addWidget(&mToggle);
    layout->addWidget(&mPlay);
    
    mLabel.setText("State : default");
    mToggle.setText("Toggle state");
    mPlay.setText("Play");

    setLayout(layout);

    connect(&mToggle, SIGNAL(pressed()), this, SLOT(toggleState()));
    connect(&mPlay, SIGNAL(pressed()), this, SLOT(playPause()));
}

HMMFrontend::~HMMFrontend()
{
    if(mModel)
        mModel->unregisterObserver(this);
}

void HMMFrontend::setModel(HMM* model)
{
    if(mModel)
        mModel->unregisterObserver(this);
    mModel = model;
}

void HMMFrontend::notify()
{
    mLabel.setText(QString("State : %1").arg(mModel->state()));
}

void HMMFrontend::toggleState()
{
    if(mModel)
        mModel->setState(1 - mModel->state());
}

void HMMFrontend::setPlayer(StreamPlayer* player)
{
    mPlayer = player;
}

void HMMFrontend::playPause()
{
    if(mPlayer)
    {
        mPlayer->togglePauseResume();
        if(mPlayer->isPaused())
            mPlay.setText("Play");
        else
            mPlay.setText("Pause");
    }
}


