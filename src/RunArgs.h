#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

enum class RunArgOptions {
    END
};

std::function<boost::any(const boost::any&)> no_op_function(){
    return [](const boost::any& val) {return val;} ;
}

class RunArgs
{
public:
    RunArgs(const int argc, const char ** argv) noexcept;
    void parse_args(); 

    RunArgs & operator()(const std::string & name, const po::value_semantic * val, 
                                    const std::string & desc, RunArgOptions option) noexcept;
    template<typename T>
    const T & operator[](RunArgOptions & key){
        return boost::any_cast<const T&>(m_value_store[key]);
    }
    
    RunArgs& operator()(const std::string &name, const po::value_semantic *val, 
                            const std::string &desc, RunArgOptions option, 
                            RunArgOptions dependOption = RunArgOptions::END, 
                            std::function<boost::any(const boost::any &)> converter = no_op_function()) noexcept;
    
private:
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
