#include <iostream>

#include <QApplication>

#include <program_options.hpp>

#include "algo/corpus.h"
#include "algo/nnetwork.h"
#include "gui/view2D.h"
#include "gui/editor.h"

namespace po = boost::program_options;

void plotHistory(float* history, int size);
int* randomPermutation(int n);

int main(int argc, char **argv)
{
    // Handling arguments
    po::options_description desc("Usage");
    desc.add_options()
        ("train,t", "Train the network")
        ("eval,e", "Compute the error rate of the network")
        ("plot,p", "Plot the network and / or the corpus")
        ("net,n", po::value<std::string>(), "Path to the network")
        ("corpus,c", po::value<std::string>(), "Path to the corpus")
        ("out,o", po::value<std::string>(), "Path to the output (default: output.png)")
        ("rate,r", po::value<float>(), "Training rate (default: 0.01)")
        ("max,m", po::value<int>(), "Maximum iterations count for training (default: 1000)")
        ("no-random-weights", "Don't randomize weights before training")
        ("no-random-samples", "Don't randomize the order of the training corpus")
        ("verbose", "Be verbose")
        ("help,h", "Display this message")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    // Program's behaviour
    if(vm.count("help"))
    {
        std::cout << "Neural network analysis tool.\n\nExamples:" << std::endl;
        std::cout << "Evaluate how accurate is a network on a corpus :\n   nnat -e --net my_network.xml --corpus my_corpus.xml\n" << std::endl;
        std::cout << "Train a network to match a corpus :\n   nnat -t --net my_network.xml --corpus my_corpus.xml\n" << std::endl;
        std::cout << "Plot a network and/or a corpus :\n   nnat -g --net my_network.xml --corpus my_corpus.xml\n" << std::endl;
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
        if(vm.count("no-random-weights") == 0)
            net.randomize();
        //float *history;
        int nbIter = corpus.train(net, rate, iterMax, 0, (vm.count("no-random-weights") == 0));// 0 : &history

        if(verbose)
            cout << "Training ended after " << nbIter << " iterations.\n";
        //plotHistory(history,iterMax*corpus.size(), 13);
    }


    if(vm.count("eval"))
    {
        float accuracy = corpus.accuracy(net, verbose);
        if(verbose)
            std::cout << "Accuracy : "<<accuracy*100<<"%" << std::endl;
        else
            cout << (int)(accuracy*100) << endl;
    }

    if(vm.count("plot"))
    {
        QApplication app(argc, argv);

        Editor view;

        if(corpusFile != "")
            view.setCorpus(&corpus);
        if(netFile != "")
            view.setNet(&net);
        cout << "Set." << endl;

        view.show();
        //view.renderToImage(outFile);

        return app.exec();
    }

    return 0;
}

