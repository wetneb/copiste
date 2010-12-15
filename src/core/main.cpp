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

/**
    État des lieux après la tempête :
    - Réimplémenter l'entrainement et l'évaluation dans Corpus.
    - Corriger la propagation des erreurs dans Neuron.
    - Réécrire des corpus de test.
    - Ajouter un appel à clean() là où ça manque.

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

    neural_value corpus[13][3] {
        { 0, 0, 1 },
        { 156, 2, 0.1 },
        { 13, 0, 1 },
        { 6, 9, 0.1 },
        { 3, 3, 1 },
        { 17, 0, 0.1 },
        { 8, 11, 1 },
        { 8, 15, 0.1 },
        { 2, 9, 1},
        { 2, 14, 0.1 },
        { 0, 10, 1 },
        { 12, 11, 0.1 },
        { 15, 15, 0.1 }
    };

    //MainWindow win;
    NNetwork network;
    network.load("networks/MLP.xml");
    vector<int> input;
    input.resize(2);

    for(unsigned int i = 0; i != 16800; ++i)
    {
        input[0] = corpus[i%13][0];
        input[1] = corpus[i%13][1];
        cout << "Learning " << input[0] << ":" << input[1] << " -> "<<corpus[i%13][2]<<endl;
        network.train(input, corpus[i%13][2], 0.1);
    }

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
