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
    - Visualisation de corpus.
    - Écriture de réseaux et corpus.
    - Créer un corpus de fichiers audio
    - Implémenter des algos fiables d'analyse audio
    - Créer un dispositif de séquençage du flux
    - Mélanger tout ça
    - Créer une analyse statique puis en temps réel…
 **/

int main(int argc, char* argv[])
{
    //QApplication app(argc, argv);
    srand(time(NULL));

    //MainWindow win;

    Corpus corpus;
    corpus.load("corpus/linear-discrimination.xml");
    NNetwork network;
    network.load("networks/MLP.xml");
    corpus.train(network, 0.1, 1000);

    /*
    for(int n = 0; n != 100; ++n)
    {
        float rate = pow(10, (n-100.0)/20.0);
        cout << rate << " ";
        int iterations = 0;
        float compliance = 0;
        for(unsigned int i=0; i != 10; ++i)
        {
            network.randomize();
            iterations += corpus.train(network, 1, 1000);
            compliance += corpus.compliance(network);
        }
        cout << iterations/10.0 << " " << compliance/10.0 << endl;
    }
    */

    CorpusView cv;
    cv.setViewPort(0.03, 0.03);
    cv.addNetworkImage(&network);
    cv.redraw();
    cv.write("output.png");

    int buf;
    cin >> buf;

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
