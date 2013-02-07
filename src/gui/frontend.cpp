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
    layout->addWidget(&mSave);
    
    mLabel.setText("State : 0");
    mToggle.setText("Toggle state");
    mPlay.setText("Pause");
    mSave.setText("Save");

    setLayout(layout);

    connect(&mToggle, SIGNAL(pressed()), this, SLOT(toggleState()));
    connect(&mPlay, SIGNAL(pressed()), this, SLOT(playPause()));
    connect(&mSave, SIGNAL(pressed()), this, SLOT(save()));
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
    {
        int next_state;
        switch(mModel->state())
        {
            case 0:
                next_state = 2;
                break;
            case 1:
                next_state = 3;
                break;
            case 2:
                next_state = 0;
                break;
            case 3:
                next_state = 1;
                break;
            default:
                next_state = 0;
        }
        mModel->setState(next_state);
        QTimer::singleShot(HMM_TRANSITION_TIME, this, SLOT(updateState()));
    }
}

void HMMFrontend::updateState()
{
    if(mModel->state() == 2)
        mModel->setState(1);
    else if(mModel->state() == 3)
        mModel->setState(0);
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

void HMMFrontend::save()
{
    if(mModel)
    {
        string fname =
         QFileDialog::getSaveFileName(this, "Save model",
                 "model.xml", "Models (*.xml)").toStdString();
        if(fname != "")
            mModel->save(fname);
    }
}


