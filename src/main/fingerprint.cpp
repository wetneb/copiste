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

#include "algo/fingerprintcompare.h"
#include "algo/hmm.h"
#include "core/fingerprinter.h"
#include "gui/frontend.h"

#include <QApplication>

/**
 * \brief Main function for fingerprinting an audio file
 * Nothing interestring here : parses arguments.
 */
int main(int argc, char **argv)
{
    po::options_description desc("Usage");
    desc.add_options()
        ("input-files", po::value< std::vector<std::string> >(), "The input files.")
        ("verbose", "Be verbose.")
        ("help,h", "Display this message");

    po::positional_options_description p;
    p.add("input-files", -1);

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

    HMM model(true);
    model.erase(2);

    QApplication app(argc, argv);
    HMMFrontend frontend;
    model.setObserver(&frontend);
    frontend.setModel(&model);

    if(vm.count("input-files"))
    {
        Fingerprinter cl(vm.count("verbose"));
        cl.setConsumer(&model);

        std::vector< std::string > inputFiles = vm["input-files"].as< std::vector<std::string> >();

        for(unsigned int i = 0; i < inputFiles.size(); i++)
        {
            cl.setUrl(inputFiles[i]);

            cl.play();
            frontend.show();
            app.exec();
            int di;
            std::cin >> di;
            model.save("TODO");
        }
    }
    else
    {
        cout << "No input file has been set. Use --help for more info."<<endl;
    }

    return 0;
}

