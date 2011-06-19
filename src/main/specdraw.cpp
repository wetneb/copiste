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
        ("input-file", "The audio file to be read.")
        ("output-file,o", po::value<std::string>()->default_value("output/spectrum.xml"), "The path to the output file.")
        ("help,h", "Display this message");

    po::positional_options_description p;
    p.add("input-file", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
              options(desc).positional(p).run(), vm);
    po::notify(vm);

    if(vm.count("help"))
    {
        std::cout << "Spectrum drawing tool.\n\nExample:" << std::endl;
        std::cout << "Draw the spectrum of a file :\n   specdraw rickroll.ogg -o spectrum.png\n" << std::endl;
        std::cout << desc << std::endl;
        return 0;
    }

    SpectrumRecorder sr;

    if(vm.count("input-file"))
    {
        string filename = vm["input-file"].as< string >();
        string output = vm["output-file"].as< string >();
        cout << "File to read : " << filename << endl;
        cout << "Target : "<<output<<endl;
        sr.compute(filename);
        sr.writeToFile(output);
    }
    return 0;
}
