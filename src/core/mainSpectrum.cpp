#include <program_options.hpp>

namespace po = boost::program_options;

#include <iostream>
using namespace std; // A supprimer (ou pas)

#include "core/spectrumRecorder.h"

/**
 * \brief Main function for computing a graphical representation of spectrum
 * Nothing interesting here : parses arguments.
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
        SpectrumRecorder sr;
        string filename = vm["input-file"].as< string >();
        cout << "File to read : " << filename << endl;
        sr.compute(filename);
        sr.writeToFile("output/spectrum.png");
    }

    return 0;
}
