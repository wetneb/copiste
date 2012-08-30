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
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

namespace po = boost::program_options;

#include <iostream>

using namespace std; // todo : delete me

#include "core/fingerprinter.h"

/**
 * \brief Main function for fingerprinting an audio file
 * Nothing interesting here : parses arguments.
 */
int main(int argc, char **argv)
{
    po::options_description desc("Usage");
    desc.add_options()
        ("input-file", "The file. The one.")
        ("pipeline,p", "The pipeline that should be used (it will be loaded from pipeline/$PIPELINE.xml)")
        ("matrix", "Save the fingerprint as a matrix rather than as a vector")
        ("verbose", "Be verbose.")
        ("help,h", "Display this message");

    po::positional_options_description p;
    p.add("pipeline", 1).add("input-file", 1);

        po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
              options(desc).positional(p).run(), vm);
    po::notify(vm);

    if(vm.count("help"))
    {
        std::cout << "Fingerprint extractor.\n\nExample:" << std::endl;
        std::cout << "Extract a fingerprint :\n   fingerprint hzcrr-lster rickroll.ogg\n" << std::endl;
        std::cout << desc << std::endl;
        return 0;
    }

    Fingerprinter cl(vm.count("verbose"));
    
    if(vm.count("input-file"))
    {
        string pipeline = "pipeline/" + (vm["pipeline"].as< string >()) + ".xml";

        if(cl.setupPipeline(pipeline))
        {
            cout << "Starting…" << endl;
            cl.compute(vm["input-file"].as< string >());

            if(vm.count("verbose") == 0)
                cout << "\e[F\e[K";
            ublas::vector<int> fgp = cl.getFingerprint();

            boost::archive::text_oarchive ar(std::cout);
            if(vm.count("matrix") != 0)
            {
                ublas::matrix<int> mat(fgp.size(), 1);
                ublas::matrix_column<ublas::matrix<int> > mr(mat, 0);
                mr = fgp;

                mat = ublas::trans(mat);
                ar & mat;
            }
            else
                ar & fgp;
            std::cout << std::endl;
        }
        else
            cout << "Failed to load the pipeline." << endl;
    }
    else
    {
        cout << "No input file has been set. Use --help for more info."<<endl;
    }

    return 0;
}
