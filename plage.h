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
