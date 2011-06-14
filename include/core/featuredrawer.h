#ifndef INCLUDED_FEATUREDRAWERH
#define INCLUDED_FEATUREDRAWERH

#include <iostream>
#include <QImage>
#include <QColor>
#include <QPainter>

#include "core/soundanalyser.h"
#include "algo/nnetwork.h"

#define FEATURE_DRAWER_DEFAULT_WIDTH 1600
#define FEATURE_DRAWER_DEFAULT_HEIGHT 600

using namespace std;

class FeatureDrawer : public SoundAnalyser
{
    public:
        //! Constructor
        FeatureDrawer(bool live = false);

        //! Destructor
        ~FeatureDrawer();

        //! Set a network that can be used to classify the samples
        void setNetwork(NNetwork *net);

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
        void computeMinMax();

        QImage mOut;
        QImage mCaption;
        bool mDrawn;
        NNetwork *mNet;

        float* mMin;
        float* mMax;
        unsigned int mMinMaxSize;
};

#endif
