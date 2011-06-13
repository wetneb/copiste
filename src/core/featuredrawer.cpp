#include "core/featuredrawer.h"

FeatureDrawer::FeatureDrawer() : SoundAnalyser(),
            mOut(FEATURE_DRAWER_DEFAULT_WIDTH, FEATURE_DRAWER_DEFAULT_HEIGHT, QImage::Format_RGB32),
            mLegende("img/legende.png"),
            mDrawn(false), mNet(0), mMin(0), mMax(0)
{
    ;
}

//! Changes the size of the output. Redraws if needed.
void FeatureDrawer::setSize(int width, int height)
{
    mOut = QImage(width, height, QImage::Format_RGB32);
    mOut.fill((unsigned int)(-1));

    if(mDrawn)
        draw();
}

//! Draws the features to an image
void FeatureDrawer::draw(string name)
{
    if(nbSamples() < 2)
        return;

    // Compute the ranges
    if(mMin)
       delete mMin;
    if(mMax)
       delete mMax;
    mMin = new float[realDimension()]; // deleted in the destructor
    mMax = new float[realDimension()]; // deleted in the destructor

    int offset = 0;
    for(unsigned int f = 0;  f < nbFeatures(); f++)
    {
        if(isUsed(f))
        {
            for(unsigned int e = 0; e < nbElems(f); e++)
            {
                mMin[offset] = features(0)[f][e];
                mMax[offset] = features(0)[f][e];

                for(unsigned int i = 1; i < nbSamples(); i++)
                {
                    if(features(i)[f][e] < mMin[offset])
                        mMin[offset] = features(i)[f][e];
                    if(features(i)[f][e] > mMax[offset])
                        mMax[offset] = features(i)[f][e];
                }

                offset++;
            }
        }
    }

    mOut.fill(QColor(0,0,0).rgb());
    QPainter painter(&mOut);

    // Grahical properties
    QColor colors[] = { Qt::red,
                        Qt::green,
                        Qt::blue,
                        Qt::cyan,
                        Qt::magenta,
                        Qt::yellow,
                        Qt::gray,
                        Qt::black };
    int bottom = 25, top = 25;
    if(mNet == 0)
        top = 0;

    // Draw lines

    // Seconds
    offset = samplingFrequency() / AUDIO_CHUNK_SIZE;
    painter.setPen(QColor(30,30,30));
    for(int i = 0; i < mOut.width(); i += offset)
        painter.drawLine(i, top, i, mOut.height() - bottom);

    // Features
    painter.setPen(QColor(60,60,60));
    offset = (mOut.height() - top - bottom) / realDimension();
    for(int i = mOut.height()-1-bottom; i >= 0; i -= offset)
        painter.drawLine(0, i, mOut.width(), i);

    // Draw the features
    offset = 0;
    for(unsigned int f = 0; f < nbFeatures(); f++)
    {
        if(isUsed(f))
        {
            for(unsigned int e = 0; e < nbElems(f); e++)
            {
                float chunk = (mOut.height() - bottom - top) / realDimension();
                float scale = (mMax[offset] - mMin[offset]) / chunk;
                float orig = mOut.height() - bottom - chunk*offset;

                painter.setPen(colors[offset%8]);

                if(mMin[offset] < mMax[offset])
                {
                    QPoint lastPoint(0,0);
                    for(int i = 0; i < mOut.width() && i < (int)nbSamples(); ++i)
                    {
                        painter.drawLine(lastPoint, QPoint(i, orig - (features(i)[f][e] - mMin[offset])/scale));
                        lastPoint = QPoint(i, orig - (features(i)[f][e] - mMin[offset])/scale);
                    }

                }

                offset++;
            }
        }
    }

    // Draw the response of the network
    if(mNet)
    {
        float currentClass = 0;
        int lastChange = 0;
        for(int i = 0; i < mOut.width() && i < (int)nbSamples(); ++i)
        {
            float response = 0;
            // Create the input vector
            vector<float> inputVector;
            for(unsigned int k = 0; k < nbFeatures(); k++)
            {
                if(isUsed(k))
                {
                    for(unsigned int j = 0; j < nbElems(k); j++)
                        inputVector.push_back(features(i)[k][j]);
                }
            }

            // Send the input vector to the network
            mNet->clean();
            response = mNet->compute(inputVector);

            // Draw the class
            if(currentClass * response <= 0 || i >= mOut.width()-1 || i >= (int)nbSamples()-1)
            {
                QColor fillColor;
                if(currentClass > 0)
                    fillColor = QColor(50, 150, 50);
                else fillColor = QColor(200, 70, 30);
                painter.fillRect(lastChange, 0, i-1, top, fillColor);

                painter.setPen(QColor(200,200,200));
                painter.drawLine(i-1, top, i-1, mOut.height()-bottom);

                lastChange = i;
            }
            currentClass = 10*response;
        }
    }

    // Draw the filename
    painter.drawImage(QPoint(mOut.width() - mLegende.width(), mOut.height() - bottom), mLegende);
    painter.setPen(QColor(255,255,255));
    painter.setFont(QFont("Arial", 15));
    painter.drawText(0, mOut.height() - bottom, mOut.width(), bottom, Qt::AlignCenter, name.c_str());
}

//! Save the drawn image to a file
void FeatureDrawer::writeToFile(string file)
{
    mOut.save(file.c_str());
}

void FeatureDrawer::setNetwork(NNetwork *net)
{
    mNet = net;
}

FeatureDrawer::~FeatureDrawer()
{
    if(mMin)
       delete mMin;
    if(mMax)
       delete mMax;
}
