/*
 * This file is part of Copiste.
 *
 * Copiste is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 *  Copiste is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Copiste.  If not, see <http://www.gnu.org/licenses/>.
 */


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
 * - Créer un corpus audio consistant (il manque des pubs)
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
        ("pipeline", "The pipeline that should be used (it will be loaded from pipeline/$PIPELINE.xml)")
        ("input-file", "The target file defining the corpus.")
        ("output-file,o", po::value<string>()->default_value("corpus/output.xml"), "The file where the XML corpus will be written.")
        ("element-length,l", po::value<int>()->default_value(0), "The length (in seconds) of each element which will be written in the corpus (0 : the whole file)")
        ("computing-offset", po::value<int>()->default_value(0), "The time (in seconds) that should be considered as the starting point of the computation (to remove the first values). The same offset is applied at the end of the file.")
        ("fp-mode,f", "Turn on fingerprint mode : just keep the last feature vector and store the values as integers")
        ("help,h", "Display this message");

    po::positional_options_description p;
    p.add("pipeline", 1).add("input-file", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
              options(desc).positional(p).run(), vm);
    po::notify(vm);

    if(vm.count("help"))
    {
        std::cout << "Corpus building tool.\n\nExample:" << std::endl;
        std::cout << "Create a corpus from a set of audio files :\n   mkcorpus hzcrr-lster Rick_Astley's_greatest_hits/target.xml -o my_corpus.xml\n" << std::endl;
        std::cout << desc << std::endl;
        return 0;
    }

    CorpusBuilder cb;
    cb.setElementLength(vm["element-length"].as< int >());
    cb.setComputingOffset(vm["computing-offset"].as< int >());
    cb.setFingerprintMode(vm.count("fp-mode") > 0);

    if(vm.count("input-file"))
    {
        string corpusPath = vm["input-file"].as< string >();
        string pipeline = "pipeline/" + (vm["pipeline"].as< string >()) + ".xml";
        string output = vm["output-file"].as< string >();

        if(cb.setupPipeline(pipeline))
        {
            cout << "Processing " << corpusPath << endl;
            cb.setVerbose(true);
            cb.setup(corpusPath);
            cb.compute();
            Corpus corpus;
            cb.write(&corpus);

            corpus.write(output);
        }
        else
            cout << "Failed to load the pipeline." << endl;
    }
    else
        cout << "No target has been set. See --help." << endl;
    return 0;
}

