#include "gui/editor.h"

#include <iostream>

Editor::Editor(QWidget *parent) : QWidget(parent)
{
    mX = 0;
    mY = 0;
    mScaleX = 1;
    mScaleY = 1;
}

void Editor::setCorpus(Corpus *corpus)
{
    if(corpus->dimension() == 2)
    {
        mCorpus = corpus;
        vector<float> bounds = corpus->bounds();
        mScaleX = (bounds[1] - bounds[0])/CORPUS_VIEW_OUTPUT_HEIGHT;
        mScaleY = (bounds[3] - bounds[2])/CORPUS_VIEW_OUTPUT_WIDTH;
        mX = bounds[0];
        mY = bounds[2];
    }
    else std::cerr << "Unable to draw a corpus of dimension "<<corpus->dimension()<<", sorry.\n";
}
