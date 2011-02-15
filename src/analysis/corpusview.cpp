#include "analysis/corpusview.h"

CorpusView::CorpusView() : mImage(CORPUS_VIEW_OUTPUT_WIDTH, CORPUS_VIEW_OUTPUT_HEIGHT, QImage::Format_RGB32),
                mPainter(&mImage)
{
    erase();
}

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

QColor colorFromValue(neural_value value)
{
    if(value < 0.001 && value > -0.001)
        return QColor(0,0,255);
    return QColor(127*(1.0+value),127*(1.0-value),0);
}

void CorpusView::redraw(bool verbose)
{
    // Ajout des échelles
    //mPainter.drawLine()

    // Chargement des points
    if(mPoint0.isNull())
        mPoint0.load(POINT_0_PATH);
    if(mPoint1.isNull())
        mPoint1.load(POINT_1_PATH);

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
                mPainter.setPen(colorFromValue(value));

                mPainter.drawPoint(x,y);
            }
        }
    }

    // Ajout du corpus, si défini
    if(mCorpus)
    {
        for(unsigned int index = 0; index != mCorpus->size(); ++index)
        {
            QPixmap *img = &mPoint0;
            if(mCorpus->elem(index)[0] == 1)
                img = &mPoint1;

            mPainter.drawPoint((mCorpus->elem(index)[1] - mOrigX)/mScaleX, (mCorpus->elem(index)[2]- mOrigY)/mScaleY);
            mPainter.drawPixmap((mCorpus->elem(index)[1] - mOrigX)/mScaleX - 5, (mCorpus->elem(index)[2]- mOrigY)/mScaleY - 5, *img);
        }
    }
}

void CorpusView::addNetworkZeros()
{
    ;
}

void CorpusView::write(string fileName, string format, bool verbose)
{
    mImage.save(fileName.c_str(), format.c_str());
    if(verbose)
        cout << "Output "<< fileName << " written.\n";
}
