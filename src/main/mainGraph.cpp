
#include <program_options.hpp>
#include <filesystem.hpp>

namespace po = boost::program_options;

#include <iostream>
using namespace std; // A supprimer (ou pas)

#include "core/featuredrawer.h"

/**
 * \brief Main function for computing a graphical representation of the features
 * Nothing interesting here : parses arguments.
 */
int main(int argc, char **argv)
{
    po::options_description desc("Usage");
    desc.add_options()
        ("input-file", "The file. The one.")
        ("pipeline,p", "The pipeline that should be used (it will be loaded from pipeline/$PIPELINE.xml)")
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
        string pipeline = "pipeline/" + (vm["pipeline"].as< string >()) + ".xml";
        if(fd.setupPipeline(pipeline))
        {
            string filename = vm["input-file"].as< string >();
            string output = string("output/"+(boost::filesystem::path(filename).filename().string())+"-features.png");
            if(vm.count("output-file"))
                output = vm["input-file"].as< string >();

            cout << "File to read : " << filename << endl;
            fd.compute(filename);
            fd.waitComputed();
            fd.draw();
            fd.writeToFile(output);
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
