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

#ifndef INCLUDED_FRONTENDH
#define INCLUDED_FRONTENDH

#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>

#include "algo/hmm.h"

//! A GUI frontend for an HMM classifier
class HMMFrontend : public QWidget, public HMMStateObserver
{
    Q_OBJECT
    public:
        HMMFrontend(QWidget* parent = 0);
        
        ~HMMFrontend();

        //! Bind the frontend to a model (set to 0 to unbind)
        void setModel(HMM* model);

        //! Reimplemented from HMMStateObserver, called on state change
        void notify();

    public slots:
        void toggleState();

    private:
        HMM* mModel;
        QPushButton mButton;
        QLabel mLabel;
};

#endif


