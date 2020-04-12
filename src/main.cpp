#include <boost/program_options.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Logger.h"

namespace po = boost::program_options;

int main(int argc, char ** argv)
{
    po::options_description desc("Allowed options");

    desc.add_options()
        ("F", po::value<char>()->default_value(' '), "Field separator")
        ("f", po::value<std::string>()->default_value(""), "Script name")
        ("v", po::value<std::vector<std::string>>()->multitoken(), "Followed with var=value, assigns value to var in the script")
        ("help", "help")
        ;

    char fieldSeparator;
    std::string phrase;

    try {
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 1;
        }

        fieldSeparator = vm["F"].as<char>();

        std::string script_file_name = vm["f"].as<std::string>();

        if(!script_file_name.empty()) {
            LOG("Script file found : " <<  script_file_name);

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
    } catch(boost::wrapexcept<po::unknown_option> & ex) {
        std::cerr << "Failed " << ex.what() << std::endl;
        return 1;
    }

    std::cout << "Field separator : " << fieldSeparator << std::endl;
    std::cout << "Phrase : " << std::endl;
    std::cout << phrase << std::endl;
}
