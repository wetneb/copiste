#include "gui/mainwindow.h"

#include <iostream>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QApplication>

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QWidget(parent),
                                                        mProg(Profile("config/inter.xml")),
                                                        mPlay(">"),
                                                        mStop("||"),
                                                        mChannel("Volume :"),
                                                        mVol(Qt::Horizontal),
                                                        mSysTray(this)
{
    // Initialisation de l'interface (aucun intérêt)
    QIcon icone("icon.png");
    QIcon logo("config/logo-france-inter.jpg");

    // Description de la plage
    setWindowTitle("NowPlaying");
    QVBoxLayout *descLayout = new QVBoxLayout;
    descLayout->addWidget(new QLabel("France Inter"));
    descLayout->addWidget(&mTitre);
    mTitre.setText("Inconnu");
    descLayout->addWidget(&mDesc);

    QHBoxLayout *iconLayout = new QHBoxLayout;
    iconLayout->addWidget(&mIcone, 0);
    mIcone.setPixmap(logo.pixmap(50,50));
    iconLayout->addLayout(descLayout, 0);
    iconLayout->addStretch(1);

    // Boutons de lecture et volume
    QHBoxLayout *toolBarLayout = new QHBoxLayout;
    toolBarLayout->addWidget(&mPlay);
    mPlay.setMaximumSize(30,30);
    toolBarLayout->addWidget(&mStop);
    mStop.setMaximumSize(30,30);
    toolBarLayout->addStretch(1);
    toolBarLayout->addWidget(&mChannel);
    mVol.setMaximumSize(100,30);
    mVol.setMinimum(0);
    mVol.setMaximum(100);
    mVol.setValue(80);
    toolBarLayout->addWidget(&mVol);

    // Disposition globale
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(iconLayout);
    mainLayout->addLayout(toolBarLayout);
    setAttribute(Qt::WA_DeleteOnClose, false);

    // Graphiques
    mainLayout->addWidget(&mOnde);
    mainLayout->addWidget(&mSpectre);

    setLayout(mainLayout);

    mSysTray.setIcon(icone);
    mSysTray.show();
    setWindowIcon(icone);

    mMenu.addAction("Configuration");
    mMenu.addAction("Credits");
    mMenu.addAction("Quitter");
    mSysTray.setContextMenu(&mMenu);

    // Temporaire
    // mSp.setUrl("http://mp3.live.tv-radio.com/franceinter/all/franceinterhautdebit.mp3");
    //mSp.setUrl("/home/berthe/Musique/Dire Straits/Sultans.mp3");
    mSp.setUrl("/home/berthe/Programmation/NowPlaying/test/sinus880.ogg");
    //mSp.setUrl("http://broadcast.infomaniak.net/tsfjazz-high.mp3");
    mSp.setGraphiqueOnde(&mOnde);
    mSp.setGraphiqueSpectre(&mSpectre);
    mSpectre.setDrawBars(true);

    // Connections d'évènements

    // Visibilité de la fenêtre
    connect(&mSysTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(sysTrayActivated(QSystemTrayIcon::ActivationReason)));
    // Gestion du menu du systray
    connect(&mMenu, SIGNAL(triggered(QAction*)), this, SLOT(handleMenu(QAction*)));
    // Gestion du bouton Play
    connect(&mPlay, SIGNAL(clicked()), &mSp, SLOT(play()));
    // Gestion du bouton Stop
    connect(&mStop, SIGNAL(clicked()), &mSp, SLOT(stop()));
    // Gestion du volume
    connect(&mVol, SIGNAL(sliderMoved(int)), &mSp, SLOT(setVol(int)));
    // Actualisation du titre
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(actualiser()));
    actualiser();
}

void MainWindow::actualiser()
{
    mTimer.stop();

    // Actualisation du programme
    mProg.refresh();

    // Mise à jour de l'UI
    Plage actuelle;
    actuelle = mProg.currentSequence();
    mTitre.setText(actuelle.mNom);
    mDesc.setText(actuelle.mDescription);

    // Planification de la prochaine MAJ
    mTimer.start(mProg.nextUpdate() * 1000);

    cout << "Actualisation... " << actuelle.mNom.toStdString()
            << " " << actuelle.mDescription.toStdString() << ". Prochaine MAJ : " << mProg.nextUpdate() << endl;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

void MainWindow::notifier()
{
    Plage actuelle = mProg.currentSequence(QDate::currentDate(), QTime::currentTime());
    QString texte = ((actuelle.mDescription != "(pas de description)") ? actuelle.mNom + " - " + actuelle.mDescription : actuelle.mNom);
    mSysTray.showMessage("NowPlaying", texte);
}

void MainWindow::handleMenu(QAction *action)
{
    if(action->text() == "Quitter")
        qApp->quit();
    if(action->text() == "Credits")
        about();
}

void MainWindow::sysTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Context)
        mMenu.show();
    else if(reason == QSystemTrayIcon::DoubleClick)
        show();
}

void MainWindow::about()
{
    QMessageBox::about(this, "A propos de NowPlaying", QString::fromUtf8("Messages divers à inscrire ici..."));
}

