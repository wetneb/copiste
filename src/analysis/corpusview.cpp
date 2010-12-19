#include "analysis/corpusview.h"

void CorpusView::setViewPort(float scaleX, float scaleY, float origX, float origY)
{
    mScaleX = scaleX;
    mScaleY = scaleY;
    mOrigX = origX;
    mOrigY = origY;
    redraw();
}

void CorpusView::erase()
{
    mImage.fill(qRgb(255,255,255));
    mCorpus = 0;
    mNetwork = 0;
    mScaleX = 1;
    mScaleY = 1;
}

void CorpusView::redraw()
{
    // Ajout des échelles
    //mPainter.drawLine()

    // Ajout du réseau, si défini
    if(mNetwork)
    {
        vector<float> inputVec;
        inputVec.resize(2);

        for(unsigned int x = 0; x != CORPUS_VIEW_OUTPUT_WIDTH; ++x)
        {
            inputVec[0] = x*mScaleX + mOrigX;
            for(unsigned int y = 0; y != CORPUS_VIEW_OUTPUT_HEIGHT; ++y)
            {
                inputVec[1] = y*mScaleY + mOrigY;

                mNetwork->clean();
                float value = mNetwork->compute(inputVec);
                mPainter.setPen(QColor(127*(1.0+value),127*(1.0-value),0));

                mPainter.drawPoint(x,y);
            }
        }
    }

    // Ajout du corpus, si défini
}

void CorpusView::addNetworkZeros()
{
    ;
}

void CorpusView::write(string fileName, string format)
{
    mImage.save(fileName.c_str(), format.c_str());
}
