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
        FeatureDrawer();

        //! Destructor
        ~FeatureDrawer();

        //! Set a network that can be used to classify the samples
        void setNetwork(NNetwork *net);

        //! Changes the size of the output. Redraws if needed.
        void setSize(int width, int height);

        //! Draws the features to an image
        void draw(string name = "");

        //! Save the drawn image to a file
        void writeToFile(string file);

    private:
        QImage mOut;
        QImage mLegende;
        bool mDrawn;
        NNetwork *mNet;

        float* mMin;
        float* mMax;
};

#endif

