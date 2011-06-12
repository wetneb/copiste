#include "core/featuredrawer.h"

//! Changes the size of the output. Redraws if needed.
void FeatureDrawer::setSize(int width, int height)
{
    mOut = QImage(width, height, QImage::Format_RGB32);
    mOut.fill((unsigned int)(-1));

    if(mDrawn)
        draw();
}

//! Draws the features to an image
void FeatureDrawer::draw()
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

    // Draw lines

    // Seconds
    offset = samplingFrequency() / AUDIO_CHUNK_SIZE;
    painter.setPen(QColor(30,30,30));
    for(int i = 0; i < mOut.width(); i += offset)
        painter.drawLine(i, 0, i, mOut.height());

    // Features
    painter.setPen(QColor(60,60,60));
    offset = mOut.height() / realDimension();
    for(int i = mOut.height()-1; i >= 0; i -= offset)
        painter.drawLine(0, i, mOut.width(), i);

    // Draw
    QColor colors[] = { Qt::red,
                        Qt::green,
                        Qt::blue,
                        Qt::cyan,
                        Qt::magenta,
                        Qt::yellow,
                        Qt::gray,
                        Qt::black };


    offset = 0;
    for(unsigned int f = 0; f < nbFeatures(); f++)
    {
        if(isUsed(f))
        {
            for(unsigned int e = 0; e < nbElems(f); e++)
            {
                float chunk = mOut.height() / realDimension();
                float scale = (mMax[offset] - mMin[offset]) / chunk;
                painter.setPen(colors[offset%8]);

                if(mMin[offset] < mMax[offset])
                {
                    QPoint lastPoint(0,0);
                    for(int i = 0; i < mOut.width() && i < (int)nbSamples(); ++i)
                    {
                        painter.drawLine(lastPoint, QPoint(i, mOut.height() - chunk*offset - (features(i)[f][e] - mMin[offset])/scale));
                        lastPoint = QPoint(i, mOut.height() - chunk*offset - (features(i)[f][e] - mMin[offset])/scale);
                        //int y = ;
                        //mOut.setPixel(i, ((y < mOut.height()) ? y : mOut.height()-1), (colors[offset%8]).rgb());
                    }
                }

                offset++;
            }
        }
    }
}

//! Save the drawn image to a file
void FeatureDrawer::writeToFile(string file)
{
    mOut.save(file.c_str());
}

FeatureDrawer::~FeatureDrawer()
{
    if(mMin)
       delete mMin;
    if(mMax)
       delete mMax;
}
