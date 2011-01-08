#include <QApplication>
#include <QFont>
#include <QPushButton>

#include "algo/nnetwork.h"
#include "gui/mainwindow.h"
#include "algo/corpus.h"
#include "analysis/corpusview.h"

/**
 * \mainpage
 *
 * Copiste devrait devenir un logiciel d'archivage radiophonique numérique. Peut-être.
 */

/**
    Bonus :
    - Visualisation de corpus : adaptation des seuils et des facteurs à la réponse du réseau
    - Écriture de réseaux et corpus.
    - Créer un corpus de fichiers audio
    - Implémenter des algos fiables d'analyse audio
    - Créer un dispositif de séquençage du flux
    - Mélanger tout ça
    - Créer une analyse statique puis en temps réel…
 **/

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    srand(time(NULL));

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
