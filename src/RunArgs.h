#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

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

inline std::function<boost::any(const boost::any&)> no_op_function(){
    return [](const boost::any& val) {return val;} ;
}

class RunArgs
{
public:
    RunArgs(const int argc, const char ** argv) noexcept;
    void parse_args( bool do_inbuild_setup = true ); 

    RunArgs & operator()(const std::string & name, const po::value_semantic * val, 
                                    const std::string & desc, RunArgOptions option) noexcept;
    const boost::any & operator[](const RunArgOptions & key){
        return m_value_store[key];
    }
    
    RunArgs& operator()(const std::string &name, const po::value_semantic *val, 
                            const std::string &desc, RunArgOptions option, 
                            RunArgOptions dependOption = RunArgOptions::END, 
                            std::function<boost::any(const boost::any &)> converter = no_op_function()) noexcept;
    
private:
    void setup_enum_values() noexcept;
    RunArgs& populate_parameter(const std::string &name, const po::value_semantic *val, 
                            const std::string &desc, RunArgOptions option, 
                            RunArgOptions dependOption = RunArgOptions::END, 
                            std::function<boost::any(const boost::any &)> converter = no_op_function()) noexcept;

    po::variables_map                                       m_vm;
    po::options_description                                 m_desc;

    std::vector<std::string>                                m_name_mapping;
    std::unordered_map<RunArgOptions, boost::any>           m_value_store;
    std::unordered_map<RunArgOptions, 
        std::pair<RunArgOptions, 
        std::function<boost::any(const boost::any &)>> >    m_dependency_mappings;

    const int                                               m_argc;
    const char **                                           m_argv;
};
