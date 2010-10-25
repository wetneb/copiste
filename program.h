#ifndef INCLUDEDPROGRAMMEH
#define INCLUDEDPROGRAMMEH

#include "plage.h"
#include "profile.h"
#include <vector>
#include <QObject>

#define DEFAULT_MAJ_TEMPS 30 // secondes

using namespace std;

/** \class Program
 * \brief Lists sound sequences extracted from the metadatas.
 */
class Program : public QObject
{
    Q_OBJECT
    public:
        Program(Profile profile) : mProfile(profile) { }

        Plage currentSequence(QDate date = QDate::currentDate(), QTime time = QTime::currentTime());
        int nextUpdate();

    public slots:
        void refresh();

    private:
        vector<Plage> mSequences;
        QString mCurrentName;
        Profile mProfile;
};

#endif
