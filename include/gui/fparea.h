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

#ifndef INCLUDED_FPAREAH
#define INCLUDED_FPAREAH

#include <QWidget>
#include <QImage>
#include <QPainter>

#include "core/fingerprintconsumer.h"

const int FPAREA_HEIGHT = 64;
const int FPAREA_WIDTH = 256;

//! Widget drawing fingerprints
class FPArea : public QWidget, public FingerprintConsumer
{
    public:
        FPArea(QWidget *parent = 0);

        //! Displays a new fingerprint
        void consumeFingerprint(fingerp fp); 

    protected:
        //! Event handling function (redraws the scene)
        void paintEvent(QPaintEvent *event);

    private:
        QImage mLastImage;
};

#endif

