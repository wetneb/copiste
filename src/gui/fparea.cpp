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

#include "gui/fparea.h"

FPArea::FPArea(QWidget *parent) :
    QWidget(parent),
    mLastImage(FPAREA_WIDTH, FPAREA_HEIGHT, QImage::Format_RGB32)
{
    setFixedSize(FPAREA_WIDTH, FPAREA_HEIGHT);

    QPainter painter(&mLastImage);
    painter.fillRect(0,0,width(),height(), QColor(127,127,127));

    update();
}

void FPArea::consumeFingerprint(fingerp fp)
{
    QImage newImage(width(),height(), QImage::Format_RGB32);
    QPainter painter(&newImage);

    int pixelsize = FPAREA_HEIGHT/32;
    painter.drawImage(QPoint(-pixelsize,0),mLastImage);
    QColor white(255,255,255), black(0,0,0);
    int mask = 1;
    for(int i = 0; i < 32; i++)
    {
        painter.fillRect(width()-pixelsize,pixelsize*i,pixelsize,pixelsize,
                (mask & fp ? white : black));
        mask = mask << 1;
    }
    mLastImage = newImage;
    update();
}

void FPArea::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.drawImage(0,0,mLastImage);
    painter.end();
}

