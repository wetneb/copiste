#include <program_options.hpp>

namespace po = boost::program_options;

#include <iostream>
using namespace std;

#include "core/classifier.h"

/**
 * \brief Main function for classifying an audio file
 * Nothing interesting here : parses arguments.
 */
int main(int argc, char **argv)
{
    po::options_description desc("Usage");
    desc.add_options()
        ("input-file", "The file. The one.")
        ("pipeline,p", "The pipeline that should be used (it will be loaded from pipeline/$PIPELINE.xml)")
        ("network,n", "The network that should be used (it will be loaded from networks/$NETWORK.xml)")
        ("verbose", "Be verbose.")
        ("help,h", "Display this message");

    po::positional_options_description p;
    p.add("pipeline", 1).add("network", 1).add("input-file", 1);

        po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
              options(desc).positional(p).run(), vm);
    po::notify(vm);

    if(vm.count("help"))
    {
        std::cout << "Audio classifier.\n\nExample:" << std::endl;
        std::cout << "Classify a file :\n   classify hzcrr-lster good-network rickroll.ogg\n" << std::endl;
        std::cout << desc << std::endl;
        return 0;
    }

    Classifier cl(vm.count("verbose"));
    NNetwork net;

    if(vm.count("input-file"))
    {
        string pipeline = "pipeline/" + (vm["pipeline"].as< string >()) + ".xml";
        string network = "networks/" + (vm["network"].as< string >()) + ".xml";

        if(cl.setupPipeline(pipeline))
        {
            if(net.load(network))
            {
                cl.setNetwork(&net);
                cl.compute(vm["input-file"].as< string >());

                if(vm.count("verbose") == 0)
                    cout << "\e[F\e[K";
                cout << cl.getClass() << endl;
            }
            else
                cout << "Failed to load the network " << network << "." << endl;
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
