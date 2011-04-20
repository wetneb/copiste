#include "features/extractor.h"

#include <program_options.hpp>

namespace po = boost::program_options;

#include <iostream>
using namespace std; // A supprimer

#include "core/soundanalyser.h"

/**
 * Plans pour la suite :
 * - optimiser la pipeline (traquer les new sans delete, optimiser les boucles…)
 * - Créer un corpus audio
 * - Automatiser la mise en forme des features en corpus
 * - Si j'ai du courage, un jour, écrire les parseurs XML en utilisant du Boost plutôt que du Qt !
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

    if(vm.count("input-file"))
    {
        cout << "Corpus path : " << vm["input-file"].as< string >() << endl;
        SoundAnalyser sa;
        sa.setVerbose(true);
        sa.setBasePath(vm["input-file"].as< string >());
        sa.compute();

        sa.waitComputed();
        cout << "Computed." << endl;
    }

    return 0;
}

