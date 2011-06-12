#ifndef INCLUDED_FEATUREDRAWERH
#define INCLUDED_FEATUREDRAWERH

#include <iostream>
#include <QImage>
#include <QColor>
#include <QPainter>

#include "core/soundanalyser.h"

#define FEATURE_DRAWER_DEFAULT_WIDTH 1600
#define FEATURE_DRAWER_DEFAULT_HEIGHT 600

using namespace std;

class FeatureDrawer : public SoundAnalyser
{
    public:
        //! Constructor
        FeatureDrawer() : SoundAnalyser(), mOut(FEATURE_DRAWER_DEFAULT_WIDTH, FEATURE_DRAWER_DEFAULT_HEIGHT, QImage::Format_RGB32),
            mDrawn(false), mMin(0), mMax(0)
            { ; }

        //! Destructor
        ~FeatureDrawer();

        //! Changes the size of the output. Redraws if needed.
        void setSize(int width, int height);

        //! Draws the features to an image
        void draw();

        //! Save the drawn image to a file
        void writeToFile(string file);

    private:
        QImage mOut;
        bool mDrawn;

        float* mMin;
        float* mMax;
};

#endif

