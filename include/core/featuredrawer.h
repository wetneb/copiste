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


#ifndef INCLUDED_FEATUREDRAWERH
#define INCLUDED_FEATUREDRAWERH

#include <iostream>
#include <sstream>

#include <QImage>
#include <QColor>
#include <QPainter>
#include <QTime> // delete me \todo

#include "core/soundanalyser.h"
#include "algo/abstractclassifier.h"

const int FEATURE_DRAWER_DEFAULT_WIDTH = 1600;
const int FEATURE_DRAWER_DEFAULT_HEIGHT = 600;
const char FEATURE_DRAWER_SPECTRUM_NAME[10] = "_spectrum";

using namespace std;

//! Draws features generated by a pipeline
class FeatureDrawer : public SoundAnalyser
{
    public:
        //! Constructor
        FeatureDrawer(bool live = false);

        //! Destructor
        ~FeatureDrawer();

        //! Set whether the spectrum should be drawn or not
        void drawSpectrum(bool draw = true);

        //! Set a network that can be used to classify the samples
        virtual void setClassifier(AbstractClassifier *net);

        //! Changes the size of the output. Redraws if needed.
        void setImageSize(int width, int height);

        //! Draws the features to an image
        void draw(string name = "", bool liveMode = false);

        //! Save the drawn image to a file
        void writeToFile(string file);

        //! Write the drawn image to a paint device
        void writeToDevice(QPaintDevice *device);

    private:
        QImage mOut;
        QImage mCaption;
        bool mDrawn;
        AbstractClassifier *mClassifier;
        bool mDrawSpectrum;

        double* mMin;
        double* mMax;
        unsigned int mMinMaxSize;
};

#endif

