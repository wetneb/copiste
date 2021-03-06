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


#include <iostream>
#include <QApplication>
#include <boost/program_options.hpp>

#include "core/liveplayer.h"
#include "algo/neuralnetwork.h"
#include "algo/fingerprintcompare.h"

namespace po = boost::program_options;

// TODO : change std::cout and cout to stderr

/**
 * \brief Main function for live stream playing
 * Nothing interesting here : parses arguments.
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    po::options_description desc("Usage");
    desc.add_options()
        ("input-file", "The URI of the stream (i.e. something like http://server.org/mystream.ogg, or file:///home/rick/hit.ogg)")
        ("pipeline,p", "The pipeline that should be used (it will be loaded from pipeline/$PIPELINE.xml)")
        ("network,n", po::value<string>(), "A network used for classification (optionnal) "
                                           "(it will be loaded from networks/$NETWORK)")
        ("fingerprint,f", po::value<string>(), "A fingerprint classifier (optionnal) "
                                               "(it will be loaded from fingerprints/$FINGERPRINT)")
        ("rate,r", po::value< float >(), "Playback rate (default : 1.0)")
        ("help,h", "Display this message");

    po::positional_options_description p;
    p.add("pipeline", 1).add("input-file", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
              options(desc).positional(p).run(), vm);
    po::notify(vm);

    if(vm.count("help"))
    {
        std::cout << "Live stream classification tool.\n\nExample:" << std::endl;
        std::cout << "Play a stream :\n   live hzcrr-lster http://rick.org/rickradio.ogg\n" << std::endl;
        std::cout << desc << std::endl;
        return 0;
    }

    if(vm.count("input-file"))
    {
        LivePlayer lp;
        NeuralNetwork net;
        float rate = 1.0;
        if(vm.count("rate"))
            rate = vm["rate"].as< float >();
        lp.setRate(rate);

        string pipeline = "pipeline/" + (vm["pipeline"].as< string >())+ ".xml";
        if(lp.setupPipeline(pipeline))
        {
            string filename = vm["input-file"].as< string >();

            if(vm.count("network"))
            {
                if(vm.count("fingerprint"))
                    cout << "Warning : Networks and fingerprints aren't compatible, using only the neural network." << endl;

                string network = "networks/" + (vm["network"].as< string >());
                if(net.fromFile(network))
                    lp.setClassifier(&net);
                else
                    cout << "Warning : Unable to load the network, disabling classification." << endl;
            }
            else if(vm.count("fingerprint"))
            {
                // TODO
                cout << "Fingerprints not supported." << endl;
                /*
                string fingerprint = "fingerprints/" + (vm["fingerprint"].as< string >());
                if(fpc.fromFile(fingerprint))
                    lp.setClassifier(&fpc);
                */
            }

            lp.show();
            lp.compute(filename);

            app.exec();
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
