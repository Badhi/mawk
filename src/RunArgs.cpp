#include <iostream>
#include <cassert>

#include "RunArgs.h"

RunArgs::RunArgs(const int argc, const char ** argv) noexcept : 
                m_desc("Allowed Options"), 
                m_name_mapping(static_cast<unsigned int>(RunArgOptions::END)),
                m_argc(argc), m_argv(argv)
{
    assert(argc && "Argument count cannot be zero");
    assert(argv && "Argument container cannot be null");
}


RunArgs& RunArgs::operator()(const std::string &name, const po::value_semantic *val, 
                            const std::string &desc, RunArgOptions option, 
                            RunArgOptions dependOption /*= RunArgOptions::END*/, 
                            std::function<boost::any(const boost::any &)> converter /*= no_op_function()*/) noexcept
{
    populate_parameter(name, val, desc, option, dependOption, converter);
    return *this;
}

RunArgs& RunArgs::populate_parameter(const std::string &name, const po::value_semantic *val, 
                            const std::string &desc, RunArgOptions option, 
                            RunArgOptions dependOption /*= RunArgOptions::END*/, 
                            std::function<boost::any(const boost::any &)> converter /*= no_op_function()*/) noexcept
{
    if (val == nullptr)
        m_desc.add_options()(name.c_str(), desc.c_str());
    else
        m_desc.add_options()(name.c_str(), val, desc.c_str());

    m_name_mapping.at(static_cast<unsigned int>(option)) = name;
    if ( dependOption != RunArgOptions::END ) {
        m_dependency_mappings[option] = std::make_pair(dependOption, converter);
    }
    return *this;
}

void RunArgs::setup_enum_values() noexcept
{
    populate_parameter("F", po::value<char>()->default_value(' '), "Field Separator", RunArgOptions::FIELD_SEPARATOR);
    populate_parameter("f", po::value<std::string>()->default_value(""), "Script name", RunArgOptions::SCRIPT_FILE);
    populate_parameter("v", po::value<std::vector<std::string>>()->multitoken(), "Followed with var=value, assigns value to var in the script", RunArgOptions::VAR);
    populate_parameter("help", nullptr, "help", RunArgOptions::HELP);
}

void RunArgs::parse_args(bool do_inbuild_setup /* = true */)
{
    if (do_inbuild_setup) 
        setup_enum_values();

    po::store(po::parse_command_line(m_argc, m_argv, m_desc), m_vm);
    po::notify(m_vm);

    if(m_vm.count("help")){
        std::cerr << m_desc << "\n"; 
        exit(1); 
    }

    for(unsigned int i = 0; i < m_name_mapping.size(); i++) {
        if (m_name_mapping[i].empty()) continue;
        auto enum_val = static_cast<const RunArgOptions>(i);

        auto store_value = m_vm[m_name_mapping.at(i)];
        m_value_store[enum_val] = store_value;

        auto dependency_info_it = m_dependency_mappings.find(enum_val);
        if ( dependency_info_it != m_dependency_mappings.end()) {
            auto dependent = (*dependency_info_it).second.first;
            auto & converter = (*dependency_info_it).second.second;
            m_value_store[dependent] = converter(store_value);
        }
    }
}
