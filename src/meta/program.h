#ifndef INCLUDEDPROGRAMMEH
#define INCLUDEDPROGRAMMEH

#include "meta/plage.h"
#include "meta/profile.h"
#include <vector>
#include <QObject>

#define DEFAULT_MAJ_TEMPS 30 // secondes
#define MINIMAL_TIME_BETWEEN_UPDATES 30 // secondes

using namespace std;

/** \class Program
 * \brief Lists sound sequences extracted from the metadatas.
 */
class Program : public QObject
{
    Q_OBJECT
    public:
        //! Default constructor. Builds the program according to the passed profile.
        Program(Profile profile) : mProfile(profile) { }

        //! Returns informations about the currently playing sequence
        Plage currentSequence(QDate date = QDate::currentDate(), QTime time = QTime::currentTime());
        //! Returns remaining time to the next needed update (in seconds)
        int nextUpdate();

    public slots:
        //! Update the informations from the source (a web document, basicly)
        void refresh();

    private:
        vector<Plage> mSequences;
        QString mCurrentName;
        Profile mProfile;
};

#endif
