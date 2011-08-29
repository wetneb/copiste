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


#include "meta/program.h"
#include "meta/plage.h"

#include <QXmlQuery>

#include <QBuffer>
#include <QByteArray>
#include <QXmlSerializer>
#include <QXmlResultItems>
#include <QStringList>
#include <iostream>

using namespace std;

void Program::refresh()
{
    QString attributes[5] = { "name", "desc", "date", "start", "end"};

    QByteArray tampon;
    QBuffer sortie(&tampon);
    sortie.open(QBuffer::ReadWrite);
    QXmlQuery requete;
    QXmlSerializer serializer(requete, &sortie);
    Plage plage;

    for(int i = 0; i != 5; ++i)
    {
        //if(mProfile.isRE(attributes[i]))
        //    continue;

        requete = mProfile.getXQ(attributes[i]);
        requete.evaluateTo(&serializer);

        if(requete.isValid())
        {
            switch(i)
            {
                case 0:
                    plage.mNom = QString::fromLatin1(sortie.buffer()).simplified();
                    if(plage.mNom.isEmpty())
                        plage.mNom = "Inconnu";
                    break;
                case 1:
                    plage.mDescription = QString::fromLatin1(sortie.buffer()).simplified();
                    if(plage.mDescription.isEmpty())
                        plage.mDescription = "(pas de description)";
                    break;
                case 2:
                    plage.mDate = QDate::fromString(QString::fromLatin1(sortie.buffer()).split(' ',QString::SkipEmptyParts)[0], Qt::ISODate);
                    break;
                case 3:
                    if(sortie.buffer().isEmpty())
                        plage.mDebut = QTime::currentTime();
                    else
                        plage.mDebut = QTime::fromString(QString(sortie.buffer()).split(' ',QString::SkipEmptyParts)[0], "hh'h'mm");
                    break;
                case 4:
                    if(sortie.buffer().isEmpty())
                        plage.mDebut = QTime(0, 0, QTime::currentTime().second() + DEFAULT_MAJ_TEMPS);
                    else
                        plage.mFin = QTime::fromString(QString(sortie.buffer()).split(' ',QString::SkipEmptyParts)[0], "hh'h'mm");
                    break;
            }
        }
        sortie.reset();
        tampon = "";
    }

    // Vérifier que la plage n'existe pas encore (critère à retenir ?)
    if(mSequences.size())
    {
        if(mSequences.end()->mFin > plage.mDebut)
            return;
    }

    // L'ajouter à la liste
    mSequences.push_back(plage);
}

Plage Program::currentSequence(QDate date, QTime time)
{
    // Récupération de la plage actuelle dans la liste
    Plage actuelle = mSequences[mSequences.size() - 1];
    for(vector<Plage>::iterator iter = mSequences.begin(); iter != mSequences.end(); ++iter)
    {
        if(iter->mDate == date && iter->mDebut < time && iter->mFin > time)
        {
            actuelle = *iter;
            break;
        }
    }

    return actuelle;
}

int Program::nextUpdate()
{
    return max(QTime::currentTime().secsTo(currentSequence().mFin), MINIMAL_TIME_BETWEEN_UPDATES);
}
