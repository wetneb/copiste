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


#include <boost/version.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <QApplication>

namespace po = boost::program_options;

#include <iostream>
#include <string>

#include "core/featuredrawer.h"


using namespace std;

/**
 * \brief Main function for computing a graphical representation of the features
 * Nothing interesting here : parses arguments.
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv, false);

    po::options_description desc("Usage");
    desc.add_options()
        ("input-file", "The file. The one.")
        ("pipeline,p", "The pipeline that should be used (it will be loaded from pipeline/$PIPELINE.xml)")
        ("network,n", po::value<string>(), "A network used for classification (optionnal) (it will be loaded from networks/$NETWORK.xml)")
        ("output-file,o", "The path to the output file (default: output/$ORIGINAL_FILENAME-features.png).")
        ("help,h", "Display this message");

    po::positional_options_description p;
    p.add("pipeline", 1).add("input-file", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
              options(desc).positional(p).run(), vm);
    po::notify(vm);

    if(vm.count("help"))
    {
        std::cout << "Features drawing tool.\n\nExample:" << std::endl;
        std::cout << "Draw the features of a file :\n   featdraw hzcrr_lster rickroll.ogg -o features.png\n" << std::endl;
        std::cout << desc << std::endl;
        return 0;
    }

    if(vm.count("input-file"))
    {
        FeatureDrawer fd;
        NeuralNetwork net;

        if(vm.count("network"))
        {
            string network = "networks/" + (vm["network"].as< string >()) + ".xml";
            if(net.fromFile(network))
                fd.setNetwork(&net);
            else
                cout << "Warning : Unable to load the network, disabling classification." << endl;
        }

        string pipeline = "pipeline/" + (vm["pipeline"].as< string >()) + ".xml";
        if(fd.setupPipeline(pipeline))
        {
            string filename = vm["input-file"].as< string >();

#if BOOST_VERSION <= 104200 //! \TODO : find at what version did the change happen	    
            string output("output/"+(boost::filesystem::path(filename).filename())+"-features.png");
#else
	    string output("output/"+(boost::filesystem::path(filename).filename().string()+"-features.png");
#endif
            if(vm.count("output-file"))
                output = vm["input-file"].as< string >();

            cout << "File to read : " << filename << endl;
            fd.compute(filename);
            fd.waitComputed();
#if BOOST_VERSION <= 104200
            fd.draw(boost::filesystem::path(filename).stem());
#else
	    fd.draw(boost::filesystem::path(filename).stem().string());
#endif
            cout << "Hey" << endl << endl; //! \TODO : delete me !
            fd.writeToFile(output);
            cout << "Done" << endl; //! \TODO : delete me !
        }
        else
            cout << "Failed to load the pipeline." << endl;
        cout << "OUT" << endl; //! \TODO : delete me !
    }
    else
    {
        cout << "No input file has been set. Use --help for more info."<<endl;
    }
    cout << "DELETED" << endl; // \TODO : delete me !

    return 0;
}
