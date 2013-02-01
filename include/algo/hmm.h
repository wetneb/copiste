/*
* This file is part of Copiste.
*
* Copiste is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
*  Copiste is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with Copiste.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INCLUDEDHMMH
#define INCLUDEDHMMH

#include "core/fingerprintconsumer.h"
#include "store/mapdb.h" // TODO : delete me (factory ?)

#include <string>
#include <deque>
#include <vector>

using namespace std;

//! An observer for the state of the Hidden Markov Model
class HMMStateObserver
{
    public:
        //! Method called after every state change
        virtual void notify() = 0;
};

//! A Hidden Markov Model classifier
class HMM : public FingerprintConsumer
{
    public:
        HMM(bool training = false);
        ~HMM();

        //! Create a fresh model with n states
        void erase(int n);
        //! Load a model from a file (return false if an error occured)
        bool load(string filename);
        //! Save a model to a file (return false if an error occured)
        bool save(string filename);

        //! Manually change the state of the model
        void setState(int state);
        //! Get the current state (inferred or set if in training mode)
        int state();

        //! Set the observer for this model
        void setObserver(HMMStateObserver *obs);
        //! Unregister the observer (only if the current one is the one passed
        // in argument)
        void unregisterObserver(HMMStateObserver *obs);

    private:
        //! Callback reimplemented from FingerprintConsumer
        void consumeFingerprint(fingerp fp);
        //! Increments the mCurrentState field of the vector
        vector<int> incrementCurrent(vector<int> orig);
        vector<int> mNullVector;
        //! Infers the current state (Viterbi algorithm)
        void infer();

        //! Training or classifying
        bool mTraining;

        // Model parameters
        int mNbStates;
        int** mMatrix;
        MapDb<vector<int> > mEmit;
        int mNbFpSeen;

        // State inference
        int mCurrentState;
        deque<fingerp> mObs;
        vector<vector<float> > mProbas;

        // Notification
        HMMStateObserver *mObserver;
};

#endif


