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


#include <program_options.hpp>

namespace po = boost::program_options;

#include <iostream>
using namespace std; // A supprimer (ou pas)

#include "core/spectrumrecorder.h"

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
        cout << endl << endl << "Computed" << endl << endl;
        sr.writeToFile(output);
        cout << "Saved."<< endl;
    }
    return 0;
}
