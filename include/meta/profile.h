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


#ifndef INCLUDEDPROFILEH
#define INCLUDEDPROFILEH

#include <QHash>
#include <QString>
#include <QXmlQuery>
#include <QRegExp>

/**
 * \class Profile
 * \brief Stores informations about both the audio source and the metadata source of a radio stream
 */
class Profile
{
    public:
        //! Default constructor. Loads a file if a source is given.
        Profile(QString source = "");

        //! Parses a profile file (XML)
        QString parse(QString source);

        //! Checks whether the given attribute will be computed from a regular expression or an XML query
        bool isRE(QString attribute);

        //! Returns the XML query for a given attribute
        QXmlQuery getXQ(QString attribute);
        //! Returns the regular expression for a given attribute
        QRegExp getRE(QString attribute);

        //! Returns the value (as a string) of a given attribute
        QString getString(QString attribute);
        //! Returns the value (as a date) of a given attribute
        QDate getDate(QString attribute);
        //! Returns the value (as a time) of a given attribute
        QTime getTime(QString attribute);

        //! Represents two modes of information extracting (not used anymore)
        enum Mode
        {
            FULL, GUESS
        };
        typedef enum Mode Mode;

        //! Sets the name of the stream
        QString mName;
        //! Sets the URL of the stream
        QString mStreamUrl;
        //! Sets the URL of the logo of the stream
        QString mLogoUrl;
        //! Sets the URL of the document containing metadata informations
        QString mMetaUrl;
        //! Sets the mode of information extracting (not used anymore)
        Mode mMode;

    private:
        QHash<QString, QString> mXQ;
        QHash<QString, QString> mRE;
};

#endif
