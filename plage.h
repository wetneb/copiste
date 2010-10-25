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
        Plage() { }

        QDate mDate;
        QTime mDebut;
        QTime mFin;
        QString mNom;
        QString mDescription;
};


#endif
