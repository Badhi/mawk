#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

enum class RunArgOptions {
    END
};

template<typename T>
std::function<T(const T&)> no_op_function(){
    return [](const T& val) {return val;} ;
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
        return boost::any_cast<const T&>(m_value_store[m_name_mapping.at(static_cast<unsigned int>(key))]);
    }
    
    template<typename T>
    RunArgs& operator()(const std::string &name, const po::value_semantic *val, 
                            const std::string &desc, RunArgOptions option, 
                            RunArgOptions dependOption = RunArgOptions::END, 
                            std::function<void(const T&)> converter = no_op_function<T>()) noexcept
    {
        m_desc.add_options()(name.c_str(), val, desc.c_str());
        m_name_mapping.at(static_cast<unsigned int>(option)) = name;
        return *this;
    }
private:
    po::variables_map m_vm;
    po::options_description m_desc;

    std::vector<std::string> m_name_mapping;
    std::unordered_map<std::string, boost::any>  m_value_store;

    const int m_argc;
    const char ** m_argv;
};
