#include <QApplication>
#include <QFont>
#include <QPushButton>

#include "algo/nnetwork.h"
#include "gui/mainwindow.h"

/**
 * \mainpage
 *
 * Copiste devrait devenir un logiciel d'archivage radiophonique numérique. Peut-être.
 */

int main(int argc, char* argv[])
{
    //QApplication app(argc, argv);

    //MainWindow win;
    NNetwork network;
    network.load("networks/sample.xml");
    vector<int> input;
    input.push_back(100);
    input.push_back(0);

    cout << network.compute(input) << endl;
    network.display();

    return 0;//app.exec();
}

/**
 * \page Todolist
 * Télécharger le fichier http://metadatas.tv-radio.com/metadatas/franceinterRSS.xml -> Fait
 * Le parser -> Fait
 * Faire l'affichage -> Presque fini
 * Lire le flux -> Fait en partie
 * Découper le flux
 */
