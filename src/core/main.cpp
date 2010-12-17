#include <QApplication>
#include <QFont>
#include <QPushButton>

#include "algo/nnetwork.h"
#include "gui/mainwindow.h"
#include "algo/corpus.h"

/**
 * \mainpage
 *
 * Copiste devrait devenir un logiciel d'archivage radiophonique numérique. Peut-être.
 */

/**
    État des lieux après la tempête :
    - Corriger les initialisations des classes critiques
    - Corriger la propagation des erreurs dans Neuron.

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


    //MainWindow win;

    Corpus corpus;
    corpus.load("corpus/linear-discrimination.xml");
    NNetwork network;
    network.load("networks/MLP.xml");
    vector<int> input;
    input.resize(2);
    /*
    for(unsigned int i = 0; i != 16800; ++i)
    {
        input[0] = corpus[i%13][0];
        input[1] = corpus[i%13][1];
        cout << "Learning " << input[0] << ":" << input[1] << " -> "<<corpus[i%13][2]<<endl;
        network.train(input, corpus[i%13][2], 0.1);
    }
        */
    corpus.train(network, 0.1, 10);
    cout <<"Trained.\n";
    network.display();

    int x = 0, y = 0;
    while(y >= 0)
    {
        cout << "Calcul interactif :\nX = ";
        cin >> x;
        cout << "Y = ";
        cin >> y;
        input[0] = x;
        input[1] = y;
        cout << network.compute(input) << endl;
        network.clean();
    }
    cin >> input[0];
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
