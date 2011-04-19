#include "features/extractor.h"

#include <program_options.hpp>

namespace po = boost::program_options;

#include <iostream>
using namespace std; // A supprimer

#include "features/streamplayer.h"

/**
 * Plans pour la suite :
 * - Implémenter la FFT et optimiser la pipeline
 * - Ajouter d'autres features
 * - Créer un corpus audio
 * - Automatiser la mise en forme des features en corpus
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
        cout << "Input file : " << vm["input-file"].as< string >() << endl;
        StreamPlayer sp;
        sp.setUrl(vm["input-file"].as< string >());
        sp.play();

        int n;
        cin >> n;
    }

    return 0;
}

