#include "core/classifier.h"

//! Constructor
Classifier::Classifier(bool verbose) : SoundAnalyser(), mNet(0), mVerbose(verbose)
{ ; }

//! Get the class of the content
string Classifier::getClass()
{
    waitComputed();
    string result = "Unknown";

    if(mNet)
    {
        // Create the input vector
        vector<float> inputVector;
        for(unsigned int i = 0; i < nbFeatures(); i++)
        {
            if(isUsed(i))
            {
                for(unsigned int j = 0; j < nbElems(i); j++)
                {
                    float sum = 0;
                    // Compute the average
                    for(unsigned int k = nbSamples()*0.1; k < nbSamples(); k++)
                        sum += features(k)[i][j];
                    inputVector.push_back(sum / (nbSamples()*0.9));

                    if(mVerbose)
                        cout << sum / (nbSamples()*0.9) << "\t";
                }
            }
        }
        if(mVerbose)
            cout << endl;

        // Send the input vector to the network
        float response = mNet->compute(inputVector);

        // Get the result
        if(response >= 0)
            result = "Speech";
        else
            result = "Music";
    }

    return result;
}

void Classifier::setNetwork(NNetwork *net)
{
    mNet = net;
}
