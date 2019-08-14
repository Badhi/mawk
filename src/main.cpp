#include <boost/program_options.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

namespace po = boost::program_options;

int main(int argc, char ** argv)
{
    po::options_description desc("Allowed options");

    desc.add_options()
        ("FS", po::value<char>(), "Field separator")
        ("f", po::value<std::string>(), "Script name")
        ("help", "help") 
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);    

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    char fieldSeparator = ' ';
    if (vm.count("FS")) {
        fieldSeparator = vm["FS"].as<char>();
    }

    std::string phrase;
    if(vm.count("f")){
        std::ifstream scriptFile(vm["f"].as<std::string>());
        std::stringstream s;
        s << scriptFile.rdbuf();
        phrase = s.str();
    } 
    else{
        phrase = argv[argc-1];
    } 

    std::cout << "Field separator : " << fieldSeparator << std::endl;
    std::cout << "Phrase : " << std::endl;
    std::cout << phrase << std::endl;
}
