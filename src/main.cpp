#include <QApplication>
#include <QFont>
#include <QPushButton>

#include "program.h"
#include "mainwindow.h"

/**
 * \mainpage
 *
 * Copiste devrait devenir un logiciel d'archivage radiophonique numérique. Peut-être.
 */

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow win;

    return app.exec();
}

/**
 * \page Todolist
 * Télécharger le fichier http://metadatas.tv-radio.com/metadatas/franceinterRSS.xml -> Fait
 * Le parser -> Fait
 * Faire l'affichage -> Presque fini
 * Lire le flux -> Fait en partie
 * Découper le flux
 */
