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
#include <QImage>
#include <QColor>
#include <QPainter>

#include "core/soundanalyser.h"
#include "algo/neuralnetwork.h"

#define FEATURE_DRAWER_DEFAULT_WIDTH 1600
#define FEATURE_DRAWER_DEFAULT_HEIGHT 600
#define FEATURE_DRAWER_SPECTRUM_NAME "_spectrum"

using namespace std;

//! Draws features generated by a pipeline
class FeatureDrawer : public SoundAnalyser
{
    public:
        //! Constructor
        FeatureDrawer(bool live = false);

        //! Destructor
        ~FeatureDrawer();

        //! Set if the spectrum should be drawn
        void drawSpectrum(bool draw = true);

        //! Set a network that can be used to classify the samples
        void setNetwork(NeuralNetwork *net);

        //! Changes the size of the output. Redraws if needed.
        void setImageSize(int width, int height);

        //! Draws the features to an image
        void draw(string name = "", bool liveMode = false);

        //! Save the drawn image to a file
        void writeToFile(string file);

        //! Write the drawn image to a paint device
        void writeToDevice(QPaintDevice *device);

    private:
        //! Compute the min and the max of the features
        void computeMinMax(int startingPoint = 0);

        QImage mOut;
        QImage mCaption;
        bool mDrawn;
        NeuralNetwork *mNet;
        bool mDrawSpectrum;

        double* mMin;
        double* mMax;
        unsigned int mMinMaxSize;
};

#endif

