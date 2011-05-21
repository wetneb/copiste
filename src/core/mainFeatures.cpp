#include "features/extractor.h"

#include <program_options.hpp>

namespace po = boost::program_options;

#include <iostream>
using namespace std; // A supprimer

#include "core/corpusbuilder.h"

/**
 * Plans pour la suite :
 * - Normaliser les données audio : rendre les mesures indépendantes de la fréquence d'échantillonnage
 *   (devrait pouvoir se faire en interne dans VLC, en spécifiant explicitement un resampler)
 * - Calibrer correctement les features, donner une mesure logarithmique du spectre
 * - Créer d'autres features : spectrum flux, high/low frequencies (not hard, but needs some time)
 * - optimiser la pipeline (traquer les new sans delete, optimiser les boucles…)
 * - Créer un corpus audio consistant (il manque des pubs)
 * - Si j'ai du courage, un jour, écrire les parseurs XML en utilisant du Boost plutôt que du Qt !
 */

 /**
  * Repères
  * 44100 Hz.
  *
  * Pour une fenêtre de 1024 :
  * durée : 23 ms
  * gamme de fréquences : de 43 Hz à 22050 Hz
  * domaine audible : de 20 Hz à 20000 Hz
  */

/**
 * Main function for computing features and writing it down into XML files
 * Nothing interesting here : just parses the arguments
 */
int main(int argc, char **argv)
{
    po::options_description desc("Usage");
    desc.add_options()
        ("input-file", "The file. The one.");

    po::positional_options_description p;
    p.add("input-file", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
              options(desc).positional(p).run(), vm);
    po::notify(vm);

    CorpusBuilder cb;

    if(vm.count("input-file"))
    {
        string corpusPath = vm["input-file"].as< string >();
        cout << "Corpus path : " << corpusPath << endl;
        cb.setVerbose(true);
        cb.setup(corpusPath);
        cb.compute();
        Corpus corpus;
        cb.write(&corpus);

        corpus.write("/tmp/output.xml");
        corpus.display();

        cout << "Computed." << endl;
    }
    cout << "Done" << endl;
    boost::posix_time::seconds tm(5);
    boost::this_thread::sleep(tm);
    cout << "Waited" << endl;
    return 0;
}

