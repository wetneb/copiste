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

#include <program_options.hpp>

#include "algo/corpus.h"
#include "algo/neuralnetwork.h"
#include "gui/view2D.h"
#include "gui/editor.h"

namespace po = boost::program_options;

void plotHistory(double* history, int size);
int* randomPermutation(int n);

int main(int argc, char **argv)
{
    // Handling arguments
    po::options_description desc("Usage");
    desc.add_options()
        ("train,t", "Train the network")
        ("eval,e", "Compute the error rate of the network")
        ("plot,p", "Plot the network and / or the corpus")
        ("net,n", po::value<std::string>(), "Path to the network. Use NEW/dim/depth to create"
                        " a new one with \"dim\" inputs and \"depth\" hidden layers.")
        ("corpus,c", po::value<std::string>(), "Path to the corpus")
        ("out,o", po::value<std::string>(), "Path to the output (default: output.png)")
        ("rate,r", po::value<double>(), "Training rate (default: 0.01)")
        ("regularization,l", po::value<double>(), "Regularization factor (default: 0.001)")
        ("iter,i", po::value<int>(), "Maximum iterations count for training (default: 1000)")
        ("no-random-weights", "Don't randomize weights before training")
        ("no-random-samples", "Don't randomize the order of the training corpus")
        ("verbose", "Be verbose (print a longer output)")
	("debug,d", "Debug mode : check the gradient")
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
    else if(vm.count("eval") == 0 && vm.count("train") == 0 && vm.count("plot") == 0)
    {
        std::cout << "No target has been set. Type --help for usage instructions." << std::endl;
        return 0;
    }

    std::string corpusFile = "", netFile = "", outFile = "out.png";
    bool verbose = vm.count("verbose");
    double rate = 0.01, regularization = 0.01;
    NeuralNetwork net;
    Corpus corpus;

    if(vm.count("net"))
    {
        netFile = vm["net"].as<std::string> ();
        if(netFile.substr(0,4) == "NEW/")
        {
	    //! \todo Document and handle errors
            std::istringstream s(netFile);

            char buf = 0;
            for(int i = 0; i < 4; i++)
                s >> buf;
            std::vector<int> geom;
	    int dim = 0;
	    do
	    {
		   s >> dim;
		   geom.push_back(dim);
		   s >> buf;
	    }
	    while(buf != '/'); 
            
            net.reset(geom);
            net.randomize();
        }
        else
            net.fromFile(netFile);
    }
    else if(vm.count("train") or vm.count("eval"))
    {
        std::cerr << "Error: no network has been set." << std::endl;
        return 1;
    }

    if(vm.count("corpus"))
    {
        corpusFile = vm["corpus"].as<std::string> ();
        corpus.load(corpusFile);
    }
    else if(vm.count("train") or vm.count("eval"))
    {
        std::cerr << "Error: no corpus has been set." << std::endl;
        return 1;
    }

    if(vm.count("rate"))
        rate = vm["rate"].as<double> ();

    if(vm.count("regularization"))
        regularization = vm["regularization"].as<double> ();

    int nbIter = 1000;
    if(vm.count("iter"))
        nbIter = vm["iter"].as<int> ();


    // Running
    if(vm.count("train"))
    {
        if(vm.count("no-random-weights") == 0)
            net.randomize();
        corpus.display();
        double cost = net.train(corpus, rate, regularization, nbIter, (vm.count("debug") != 0)); // &history, (vm.count("no-random-samples") == 0), (vm.count("verbose") == 1));// 0 : &history

        if(verbose)
            std::cout << "Training ended with cost " << cost << "\n";
        // plotHistory(history,iterMax*corpus.size(), 13);

        net.toFile("trained-network.xml");
    }


    if(vm.count("eval"))
    {
        double accuracy = net.accuracy(corpus);
        if(verbose)
            std::cout << "Accuracy : "<<accuracy*100<<"%" << std::endl;
        else
            std::cout << (int)(accuracy*100) << std::endl;
    }

    if(vm.count("plot"))
    {
        QApplication app(argc, argv);

        Editor view;

        if(corpusFile != "")
            view.setCorpus(&corpus);
        else view.setCorpus(new Corpus(2));
        if(netFile != "")
            view.setNet(&net);
        view.setTrainingRate(rate);
        view.setRegularization(regularization);
        view.setIter(nbIter);
	view.setDebug(vm.count("debug") != 0);

        view.show();
        //view.renderToImage(outFile);

        return app.exec();
    }

    return 0;
}

