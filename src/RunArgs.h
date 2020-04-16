#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <exception>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

class RunArgNoFoundException : public std::runtime_error
{
public:
    RunArgNoFoundException(const std::string & error_msg) : std::runtime_error(error_msg) {}
};


enum class RunArgOptions {
    FIELD_SEPARATOR = 0,
    SCRIPT_FILE,
    VAR,
    BYTE,
    TRADITIONAL,
    DUMP_VARIABLES,
    EXEC,
    HELP,
    INCLUDE_FILES,
    END
};

inline std::ostream & operator<<(std::ostream & os,  const RunArgOptions  & runarg) {
    switch(runarg) {
        case RunArgOptions::FIELD_SEPARATOR : return os << "FIELD_SEPARATOR";
        case RunArgOptions::SCRIPT_FILE : return os << "SCRIPT_FILE ";
        case RunArgOptions::VAR : return os << "VAR";
        case RunArgOptions::BYTE : return os << "BYTE";
        case RunArgOptions::TRADITIONAL: return os << "TRADITIONA";
        case RunArgOptions::DUMP_VARIABLES : return os << "DUMP_VARIABLES";
        case RunArgOptions::EXEC : return os << "EXEC";
        case RunArgOptions::HELP : return os << "HELP";
        case RunArgOptions::INCLUDE_FILES : return os << "INCLUDE_FILES";
        case RunArgOptions::END : return os << "END";
        default : return os << "UNDEFINED"; 
    }
    return os;
}

inline std::function<po::variable_value(const po::variable_value&)> no_op_function(){
    return [](const po::variable_value& val) {return val;} ;
}

class RunArgs
{
public:
    RunArgs(const int argc, const char ** argv) noexcept;
    void parse_args( bool do_inbuild_setup = true ); 

    RunArgs & operator()(const std::string & name, const po::value_semantic * val, 
                                    const std::string & desc, RunArgOptions option) noexcept;
    const po::variable_value & operator[](const RunArgOptions & key){
        try {
            return m_value_store.at(key);
        } catch(std::exception & v) {
            std::stringstream s; s << "Cannot find the Run Arg for " << key;
            throw RunArgNoFoundException(s.str());
        }
    }
    
    RunArgs& operator()(const std::string &name, const po::value_semantic *val, 
                            const std::string &desc, RunArgOptions option, 
                            RunArgOptions dependOption = RunArgOptions::END, 
                            std::function<po::variable_value(const po::variable_value &)> converter = no_op_function()) noexcept;
    
private:
    void setup_enum_values() noexcept;
    RunArgs& populate_parameter(const std::string &name, const po::value_semantic *val, 
                            const std::string &desc, RunArgOptions option, 
                            RunArgOptions dependOption = RunArgOptions::END, 
                            std::function<po::variable_value(const po::variable_value &)> converter = no_op_function()) noexcept;

    po::variables_map                                       m_vm;
    po::options_description                                 m_desc;

    std::vector<std::string>                                m_name_mapping;
    std::unordered_map<RunArgOptions, po::variable_value>           m_value_store;
    std::unordered_map<RunArgOptions, 
        std::pair<RunArgOptions, 
        std::function<po::variable_value(const po::variable_value &)>> >    m_dependency_mappings;

    const int                                               m_argc;
    const char **                                           m_argv;
};
