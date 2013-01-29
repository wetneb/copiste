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

#include "algo/hmm.h"

HMM::HMM(bool training) : mTraining(training), mNbStates(0),
            mMatrix(0), mNbFpSeen(0), mCurrentState(0)
{
    ;
}

HMM::~HMM()
{
    if(mMatrix)
    {
        for(int i = 0; i < mNbStates; i++)
            if(mMatrix[i])
                delete mMatrix[i];
        delete mMatrix;
    }
}

void HMM::erase(int n)
{
    mNbStates = n;
    mMatrix = new int*[n];
    for(int i = 0; i < n; i++)
        mMatrix[i] = new int[n];
    mNbFpSeen = 0;
}

bool HMM::load(string filename)
{

    return true;
}

bool HMM::save(string filename)
{

    return true;
}

void HMM::setState(int state)
{
    if(mTraining && mNbFpSeen > 0)
    {
        mMatrix[mCurrentState][state]++;
        mMatrix[mCurrentState][mCurrentState]--;
    }
    mCurrentState = state;
}

void HMM::consumeFingerprint(fingerp fp)
{
    ;
}

