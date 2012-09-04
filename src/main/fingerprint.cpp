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


#include <boost/program_options.hpp>
#include <boost/any.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

namespace po = boost::program_options;

#include <iostream>

#include "core/fingerprinter.h"

/*
 * TODO :
 * - add the ability to specify only a directory, as in mkcorpus. Maybe with the help of a 'target.xml'-like ?
 * - add the ability to give names to the outputs (right now it only depends on the order of computation…)
 *   it may require to add a "name(i)" field in FeatureExtractor (and so, to delete them from SoundAnalyser)
 */

/**
 * \brief Main function for fingerprinting an audio file
 * Nothing interestring here : parses arguments.
 */
int main(int argc, char **argv)
{
    po::options_description desc("Usage");
    desc.add_options()
        ("input-files", po::value< std::vector<std::string> >(), "The input files.")
        ("pipeline,p", po::value< std::string >(), "The pipeline that should be used (it will be loaded from pipeline/$PIPELINE.xml)")
        ("vector", "Save the fingerprint as a vector rather than as a matrix.")
        ("verbose", "Be verbose.")
        ("help,h", "Display this message");

    po::positional_options_description p;
    p.add("pipeline", 1).add("input-files", -1);

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

    if(vm.count("input-files") && vm.count("pipeline"))
    {
        std::string pipeline = "pipeline/" + (vm["pipeline"].as< std::string >()) + ".xml";

        if(cl.setupPipeline(pipeline))
        {
            std::vector< std::string > inputFiles = vm["input-files"].as< std::vector<std::string> >();

            ublas::matrix<int> mat(cl.realDimension(), inputFiles.size());
            boost::archive::text_oarchive ar(std::cout);

            for(unsigned int i = 0; i < inputFiles.size(); i++)
            {
                cl.compute(inputFiles[i]);

                ublas::vector<int> fgp = cl.getFingerprint();

                if(vm.count("vector") == 0)
                {
                    ublas::matrix_column<ublas::matrix<int> > mr(mat, i);
                    mr = fgp;
                }
                else
                {
                    ar & fgp;
                    std::cout << std::endl;
                }
            }

            mat = ublas::trans(mat);
            ar & mat;
        }
        else
            cout << "Failed to load the pipeline." << endl;
    }
    else
    {
        cout << "No input file or no pipeline has been set. Use --help for more info."<<endl;
    }

    return 0;
}

