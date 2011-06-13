#ifndef INCLUDED_CLASSIFIER
#define INCLUDED_CLASSIFIER

#include "core/soundanalyser.h"
#include "algo/nnetwork.h"

//! A class reading a file and classifying its content
class Classifier : public SoundAnalyser
{
    public:
        //! Constructor
        Classifier(bool verbose = true);

        //! Set the network that should be used for classification
        void setNetwork(NNetwork *net);

        //! Get the class of the content
        string getClass();

    private:
        NNetwork *mNet;
        bool mVerbose;
};

#endif
