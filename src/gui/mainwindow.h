#ifndef INCLUDEDFENETREH
#define INCLUDEDFENETREH

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QMenu>
#include <QPushButton>
#include <QSystemTrayIcon>
#include <QSlider>

#include "meta/program.h"
#include "core/streamplayer.h"
#include "meta/profile.h"
#include "gui/graphique.h"

/**
 * \class MainWindow
 * \brief Defines GUI and event connections
 */
class MainWindow : public QWidget
{
    Q_OBJECT
    public:
        //! Default constructor. Creates the main UI
        MainWindow(QWidget *parent = 0);

    protected:
        //! Handles window closing so the program can run in background
        void closeEvent(QCloseEvent *event);

    public slots:
        //! Updates the UI with new metadatas
        void actualiser();
        //! Notifies some changes to the user via infoboxes
        void notifier();
        //! Handles any click on the system tray menu
        void handleMenu(QAction *action);
        //! Handles any click on the system tray
        void sysTrayActivated(QSystemTrayIcon::ActivationReason reason);
        //! Dispays an "about" dialog
        void about();

    private:
        Program mProg;
        StreamPlayer mSp;

        // Interface globale
        QLabel mTitre;
        QLabel mDesc;
        QLabel mIcone;
        QPushButton mPlay;
        QPushButton mStop;
        QLabel mChannel;
        QSlider mVol;
        QTimer mTimer;

        // Systray
        QSystemTrayIcon mSysTray;
        QMenu mMenu;

        // Visualisation de debug
        Graphique mOnde;
        Graphique mSpectre;
};

#endif
