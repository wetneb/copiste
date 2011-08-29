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


#include "meta/profile.h"

#include <QDomDocument>
#include <QFile>
#include <QBuffer>
#include <QByteArray>
#include <QXmlSerializer>
#include <QXmlResultItems>
#include <QStringList>
#include <QDate>
#include <QTime>

#include <iostream>
using namespace std;

Profile::Profile(QString source)
{
    if(!source.isEmpty())
        parse(source);
}

bool Profile::isRE(QString attribute)
{
    return mRE.contains(attribute);
}

QXmlQuery Profile::getXQ(QString attribute)
{
    QXmlQuery xq;
    xq.setQuery(mXQ[attribute]);
    return xq;
}

QRegExp Profile::getRE(QString attribute)
{
    QRegExp re;
    re.setPattern(attribute);
    return re;
}

QString Profile::getString(QString attribute)
{
    QString result;
    QByteArray buff;
    QBuffer out(&buff);
    out.open(QBuffer::ReadWrite);

    if(isRE(attribute))
    {

    }
    else
    {
        QXmlQuery xq;
        QXmlSerializer serializer(xq, &out);
        xq.setQuery(mXQ[attribute]);
        result = QString::fromLatin1(out.buffer());
    }
    return result;
}

QDate Profile::getDate(QString attribute)
{
    return QDate::fromString(this->getString(attribute).split(' ',QString::SkipEmptyParts)[0], Qt::ISODate);;
}

QTime Profile::getTime(QString attribute)
{
    return QTime::fromString(this->getString(attribute).split(' ',QString::SkipEmptyParts)[0], "hh'h'mm");;
}

QString Profile::parse(QString source)
{
    QDomDocument doc;

    QFile file(source);
    if(!file.open(QIODevice::ReadOnly))
        return QString("Impossible d'ouvrir ")+source;
    doc.setContent(&file);
    file.close();

    QDomNode node = doc.documentElement().firstChild();

    mName = doc.documentElement().attribute("name", "Station inconnue");

    while(!node.isNull())
    {
         QDomElement elem = node.toElement();
         if(!elem.isNull())
         {
             if(elem.tagName() == "stream")
             {
                 mStreamUrl = elem.attribute("url");
             }
             else if(elem.tagName() == "logo")
             {
                 mLogoUrl = elem.attribute("url");
             }
             else if(elem.tagName() == "meta")
             {
                 mMetaUrl = elem.attribute("url");

                 QDomNode queriesNode = node.firstChild();
                 while(!queriesNode.isNull())
                 {
                     QDomElement queriesElem = queriesNode.toElement();

                     if(!queriesElem.isNull())
                     {
                         if(queriesElem.tagName() == "mode")
                         {
                             if(queriesElem.text() == "Full")
                                mMode = FULL;
                             else
                                mMode = GUESS;
                         }
                         else if(queriesElem.tagName() == "xquery")
                         {
                              mXQ.insert(queriesElem.attribute("attribute","undefined"), queriesElem.text());
                         }
                         else if(elem.tagName() == "regexp")
                             mRE.insert(queriesElem.attribute("attribute","undefined"), queriesElem.text());
                     }
                     queriesNode = queriesNode.nextSibling();
                 }
             }
         }
         node = node.nextSibling();
    }
    return source;
}
