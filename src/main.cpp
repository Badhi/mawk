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
        ("field-separator,F", po::value<char>(), "Field separator")
        ("file,f", po::value<std::string>(), "Script name")
        ("val,v", po::value<std::vector<std::string>>()->multitoken(), "Followed with var=value, assigns value to var in the script")
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
    if (vm.count("F")) {
        fieldSeparator = vm["F"].as<char>();
    }

    std::string phrase;
    if(vm.count("f")){
        std::string script_file_name = vm["f"].as<std::string>();
        std::ifstream script_file(script_file_name);
        std::stringstream s;
        if(!script_file.is_open()){
            std::cerr << "Unable to open script file : " << script_file_name << std::endl; 
            return 1;
        }
        s << script_file.rdbuf();
        phrase = s.str();
    } 
    else{
        phrase = argv[argc-1];
    } 

    std::cout << "Field separator : " << fieldSeparator << std::endl;
    std::cout << "Phrase : " << std::endl;
    std::cout << phrase << std::endl;
}
