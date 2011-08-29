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


#ifndef INCLUDEDPLAGEH
#define INCLUDEDPLAGEH

#include <QDate>
#include <QTime>

/**
 * \class Plage
 * \brief Stores informations (metadata) about a sound sequence
 */
class Plage
{
    public:
        //! Default constructor. Does nothing.
        Plage() { }

        //! Date of the sound sequence
        QDate mDate;
        //! Start of the sequence
        QTime mDebut;
        //! End of the sequence
        QTime mFin;
        //! Name of the sequence
        QString mNom;
        //! Brief description of the sequence
        QString mDescription;
};


#endif
