#include <iostream>

#include <QApplication>

#include <program_options.hpp>

#include "algo/corpus.h"
#include "algo/nnetwork.h"
#include "analysis/corpusview.h"


namespace po = boost::program_options;

int main(int argc, char **argv)
{
    // Handling arguments
    po::options_description desc("Usage");
    desc.add_options()
        ("train,t", "Train the network")
        ("eval,e", "Compute the error rate of the network")
        ("graph,g", "Draw the graph of the network")
        ("net,n", po::value<std::string>(), "Path to the network")
        ("corpus,c", po::value<std::string>(), "Path to the corpus")
        ("out,o", po::value<std::string>(), "Path to the output (default: output.png)")
        ("rate,r", po::value<float>(), "Training rate (default: 0.01)")
        ("max,m", po::value<int>(), "Maximum iterations count for training (default: 1000)")
        ("no-random", "Don't randomize weights before training")
        ("verbose", "Be verbose")
        ("help,h", "Display this message")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    // Program's behaviour
    if(vm.count("help"))
    {
        std::cout << "Neural network analysis tool." << std::endl;
        std::cout << desc << std::endl;
        return 0;
    }
    else if(vm.count("eval") == 0 && vm.count("train") == 0 && vm.count("graph") == 0)
    {
        std::cout << "No target has been set. Type --help for usage instructions." << std::endl;
        return 0;
    }

    string corpusFile = "", netFile = "", outFile = "out.png";
    bool verbose = vm.count("verbose");
    float rate = 0.01;
    int iterMax = 1000;
    NNetwork net;
    Corpus corpus;

    if(vm.count("net"))
    {
        netFile = vm["net"].as<string> ();
        net.load(netFile);
    }
    else if(vm.count("train") or vm.count("eval"))
    {
        std::cerr << "Error: no network has been set." << std::endl;
        return 1;
    }

    if(vm.count("corpus"))
    {
        corpusFile = vm["corpus"].as<string> ();
        corpus.load(corpusFile);
    }
    else if(vm.count("train") or vm.count("eval"))
    {
        std::cerr << "Error: no corpus has been set." << std::endl;
        return 1;
    }

    if(vm.count("rate"))
        rate = vm["rate"].as<float> ();

    if(vm.count("max"))
        iterMax = vm["max"].as<int> ();

    // Running
    if(vm.count("train"))
    {
        if(vm.count("no-random") == 0)
            net.randomize();
        cout << "Training ended after " << corpus.train(net, rate, iterMax) << " iterations.\n";
    }


    if(vm.count("eval"))
    {
        float accuracy = corpus.accuracy(net, verbose);
        std::cout << "Accuracy : "<<accuracy*100<<"%" << std::endl;
    }

    if(vm.count("graph"))
    {
        QApplication app(argc, argv);
        CorpusView view;

        vector<int> bounds = corpus.bounds();

        /*
        cout << "Bounds for this corpus :\n";
        cout << "Min X : "<< bounds[0] << endl << "Max X : " << bounds[1] << endl << "Min Y : " << bounds[2] << "\nMax Y : " << bounds[3] << endl;
        // */

        view.setViewPort((bounds[1] - bounds[0])/(float)CORPUS_VIEW_OUTPUT_HEIGHT,
                         (bounds[3] - bounds[2])/(float)CORPUS_VIEW_OUTPUT_WIDTH,
                         bounds[0], bounds[2]);

        if(corpusFile != "")
            view.setCorpus(&corpus);
        if(netFile != "")
            view.addNetworkImage(&net);

        view.redraw(verbose);

        view.write(outFile, "PNG", verbose);
    }

    return 0;
}
