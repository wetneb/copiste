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


#include "core/featuredrawer.h"

FeatureDrawer::FeatureDrawer(bool live) : SoundAnalyser(live),
            mOut(FEATURE_DRAWER_DEFAULT_WIDTH, FEATURE_DRAWER_DEFAULT_HEIGHT, QImage::Format_RGB32),
            mCaption("img/legende.png"),
            mDrawn(false), mClassifier(0), mDrawSpectrum(true), mMin(0), mMax(0), mMinMaxSize(0)
{
    setNormalization(0,1);
}

//! Changes the size of the output. Redraws if needed.
void FeatureDrawer::setImageSize(int width, int height)
{
    mOut = QImage(width, height, QImage::Format_RGB32);
    mOut.fill((unsigned int)(-1));

    if(mDrawn)
        draw();
}

/** \todo Refactor this code (create independent functions,  offsets…) */

//! Draws the features to an image
void FeatureDrawer::draw(string filename, bool live)
{
    if(nbSamples() < 2)
        return;

    mOut.fill(QColor(0,0,0).rgb());
    QPainter painter(&mOut);

    // Grahical properties
    QColor colors[] = { Qt::red,
                        Qt::green,
                        Qt::blue,
                        Qt::cyan,
                        Qt::magenta,
                        Qt::yellow };
    int bottom = 25, top = 25;
    if(mClassifier == 0)
        top = 0;

    // Where should we start plotting ?
    int plotStart = 0;
    if(live)
    {
        if((int)nbSamples() - (int)mOut.width() >= 0)
            plotStart = ((int)nbSamples() - (int)mOut.width());
    }

    // Should we draw spectrum ?
    if(mDrawSpectrum)
    {
        mDrawSpectrum = (getFeatureByName(FEATURE_DRAWER_SPECTRUM_NAME) != -1);
    }

    // Draw lines

    // Seconds
    double a_second = ((double)samplingFrequency()) / chunkSize();
    painter.setPen(QColor(30,30,30));
    int linesStart = plotStart % (int)a_second;
    for(int i = 1; linesStart + a_second * i < mOut.width() && linesStart + a_second * i <= nbSamples(); i++)
        painter.drawLine(linesStart + a_second*i, top, linesStart + a_second*i, mOut.height() - bottom);

    // Features
    painter.setPen(QColor(60,60,60));
    int offset = (mOut.height() - top - bottom) / (realDimension() + (mDrawSpectrum ? 1 : 0));
    for(int i = mOut.height()-1-bottom; i >= 0; i -= offset)
        painter.drawLine(0, i, mOut.width(), i);

    // Draw the features
    offset = 0;
    int nbChunks = realDimension() + (mDrawSpectrum ? 1 : 0);
    double chunk = ((nbChunks > 0) ? ((mOut.height() - bottom - top) / nbChunks) : 0);
    for(unsigned int f = 0; f < nbFeatures(); f++)
    {
        if(isUsed(f))
        {
            for(unsigned int e = 0; e < nbElems(f); e++)
            {
                double orig = mOut.height() - bottom - chunk*offset;
                
                if(isDrawnWithLines(f))
                {
                    painter.setPen(colors[offset%6]);

                    QPoint lastPoint(0,0);
                    for(int i = plotStart; i - plotStart < mOut.width() && i < (int)nbSamples(); ++i)
                    {
                        double val = features(i)[f][e];

                        if(i != plotStart)
                            painter.drawLine(lastPoint, QPoint(i - plotStart, orig - val*chunk));
                        lastPoint = QPoint(i - plotStart, orig - val*chunk);
                    }
                }
                else
                {
                    for(int i = plotStart; i - plotStart < mOut.width() && i < (int)nbSamples(); ++i)
                    {
                        int shade = std::min(255, (int)(features(i)[f][e] * 255));
                        painter.fillRect(i - plotStart, orig - chunk, 1, chunk, QColor(shade,shade,shade));
                    }
                }

                offset++;
            }
        }
        else if(name(f) == "_spectrum" && mDrawSpectrum)
        {
            int nbFrequenciesPerPixel = nbElems(f) / (3*chunk);
            if(nbFrequenciesPerPixel == 0)
                 nbFrequenciesPerPixel = 1;

            double orig = mOut.height() - bottom - chunk*offset;
            for(int i = plotStart; i - plotStart < mOut.width() && i < (int)nbSamples(); ++i)
            {
                float currentValue = 0;
                for(unsigned int e = 0; e < nbElems(f)/3 && (e / nbFrequenciesPerPixel) < chunk; e++)
                {
                    currentValue += features(i)[f][e];
                    if((e+1) % nbFrequenciesPerPixel == 0)
                    {
                        currentValue /= nbFrequenciesPerPixel;
                        currentValue *= 512;
                        currentValue = ((currentValue >= 256) ? 255 : currentValue);
                        painter.setPen(QColor(currentValue, currentValue, currentValue));
                        painter.drawPoint(i - plotStart, orig - (e / nbFrequenciesPerPixel));
                        currentValue = 0;
                    }
                }
            }
            offset++;
        }
    }

    // Draw the response of the network
    if(mClassifier)
    {
        int currentClass = 0;
        int lastChange = 0;
        for(int i = plotStart; i - plotStart < mOut.width() && i - plotStart < (int)nbSamples(); ++i)
        {
            int response = 0;
            // Create the input vector
            vector<double> inputVector;
            for(unsigned int k = 0; k < nbFeatures(); k++)
            {
                if(isUsed(k))
                {
                    for(unsigned int j = 0; j < nbElems(k); j++)
                        inputVector.push_back(features(i)[k][j]);
                }
            }

            // Send the input vector to the network
            response = mClassifier->classify(inputVector, currentClass);

            // Draw the class
            if((currentClass - 0.5) * (response - 0.5) <= 0 ||
		          i - plotStart == mOut.width()-1 ||
			  i - plotStart == (int)nbSamples()-1)
            {
                QColor fillColor;
                if(currentClass > 0.5)
                    fillColor = QColor(50, 150, 50);
                else fillColor = QColor(200, 70, 30);
                painter.fillRect(lastChange, 0, i - plotStart - lastChange, top, fillColor);

                painter.setPen(QColor(200,200,200));
                //painter.drawLine(i-1 - plotStart, top, i-1 - plotStart, mOut.height()-bottom);

                lastChange = i - plotStart;
            }
            currentClass = response;
        }
    }

    // Draw the filename
    painter.setPen(QColor(255,255,255));
    painter.setFont(QFont("Arial", (live ? 10 : 15)));
    painter.drawText(0, mOut.height() - bottom, mOut.width(), bottom, Qt::AlignCenter, filename.c_str());

    // Draw the caption
    if(mClassifier != 0)
        painter.drawImage(QPoint(mOut.width() - mCaption.width(), mOut.height() - bottom), mCaption);
    offset = 0;
    for(unsigned int f = 0; f < nbFeatures(); f++)
    {
        if(isUsed(f))
        {
            for(unsigned int e = 0; e < nbElems(f); e++)
            {
                if(isDrawnWithLines(offset))
                {
                    double origV = mOut.height() - bottom - chunk*offset;
                    int origH = (live ? 160 : mOut.width());
                    std::stringstream displayedName;
                    displayedName << name(f);
                    if(nbElems(f) > 1)
                        displayedName << " (" << e+1 <<")";

                    painter.setPen(colors[offset%8]);
                    painter.drawLine(origH - 150, origV - 0.5*chunk, origH - 135, origV - 0.5*chunk);
                    painter.setPen(QColor(255,255,255));
                    painter.drawText(origH - 130, origV - 0.5*chunk - 20, 120, 40,
                                  Qt::AlignLeft | Qt::AlignVCenter, displayedName.str().c_str());

                    offset++;
                }
            }
        }
        else if(name(f) == "_spectrum" && mDrawSpectrum)
        {
            offset++;
        }
    }

    painter.end();
}

//! Save the drawn image to a file
void FeatureDrawer::writeToFile(string file)
{
    mOut.save(file.c_str());
}

//! Write the drawn image to a paint device
void FeatureDrawer::writeToDevice(QPaintDevice *device)
{
    QPainter painter(device);
    painter.drawImage(0, 0, mOut);
}

void FeatureDrawer::setClassifier(AbstractClassifier *cl)
{
    if(cl->dimension() == this->realDimension())
        mClassifier = cl;
    else 
        std::cerr << "Error : the input dimension of the classifier (" 
         << cl->dimension() << ") and the output dimension of the pipeline ("
         << this->realDimension() << ") differ." << std::endl;
}

FeatureDrawer::~FeatureDrawer()
{
    ;
}
