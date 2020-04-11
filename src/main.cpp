#include <boost/program_options.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

namespace po = boost::program_options;

enum class OptionsIndex {
    FIELD_SEPARATOR = 0,
    SCRIPT_FILE,
    VARIABLES,
    END
};

class RunOptions{
    private:
        std::vector<std::string> value_key_store;
        std::vector<std::string> value_store;
        po::options_description desc;

        RunOptions() : value_key_store(static_cast<unsigned int>(OptionsIndex::END)), value_store(static_cast<unsigned int>(OptionsIndex::END)), desc("Allowed options"){
        }
    public:
        static RunOptions&  getInst() { 
            static RunOptions options;
            return options;
        }

        RunOptions & operator()(const char* option_name, const po::value_semantic * s, const char*  description, OptionsIndex index){
            desc.add_options()(option_name, s, description);
            value_key_store[static_cast<unsigned int>(index)] = option_name;
            return *this;
        }

};

#define ADD_OPTION(display_name, type, description, options_index) \
    RunOptions::getInst()(display_name, tpe, description, options_index) 
    
    

int main(int argc, char ** argv)
{

    RunOptions::getInst()("field-separator,F", po::value<char>(), "Field separator")
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
