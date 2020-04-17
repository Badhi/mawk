#include <iostream>
#include <cassert>

#include "RunArgs.h"

RunArgs::RunArgs(const int argc, const char ** argv) noexcept : 
                m_parsing_completed(false),
                m_desc("Allowed Options"), 
                m_name_mapping(static_cast<unsigned int>(RunArgOptions::END)),
                m_argc(argc), m_argv(argv)
{
    assert(argc && "Argument count cannot be zero");
    assert(argv && "Argument container cannot be null");
}

const po::variable_value & RunArgs::operator[](const RunArgOptions & key)
{
    if (!m_parsing_completed) {
        throw RunArgNoFoundException("Please do parse_args before getting value");
    }
    try {
        return m_value_store.at(key);
    } catch(std::exception & v) {
        std::stringstream s; s << "Cannot find the Run Arg for " << key;
        throw RunArgNoFoundException(s.str());
    }
}

RunArgs& RunArgs::operator()(const std::string &name, const po::value_semantic *val, 
                            const std::string &desc, RunArgOptions option, 
                            RunArgOptions dependOption /*= RunArgOptions::END*/, 
                            std::function<po::variable_value(const po::variable_value &)> converter /*= no_op_function()*/) noexcept
{
    if (val == nullptr)
        m_desc.add_options()(name.c_str(), desc.c_str());
    else
        m_desc.add_options()(name.c_str(), val, desc.c_str());

    m_name_mapping.at(static_cast<unsigned int>(option)) = name.substr(0, name.find_last_of(','));

    if ( dependOption != RunArgOptions::END ) {
        m_dependency_mappings[option] = std::make_pair(dependOption, converter);
    }
    return *this;
}


void RunArgs::setup_enum_values() noexcept
{
    (*this)("field-separator,F", po::value<char>()->default_value(' '), "Field Separator", RunArgOptions::FIELD_SEPARATOR);
    (*this)("file,f", po::value<std::string>()->default_value(""), "Script name", RunArgOptions::SCRIPT_FILE);
    (*this)("assign,v", po::value<std::vector<std::string>>()->multitoken(), "Followed with var=value, assigns value to var in the script", RunArgOptions::VAR);
    (*this)("help", nullptr, "help", RunArgOptions::HELP);
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

        const auto & store_value = m_vm[m_name_mapping.at(i)];
        m_value_store[enum_val] = store_value;

        auto dependency_info_it = m_dependency_mappings.find(enum_val);
        if ( dependency_info_it != m_dependency_mappings.end()) {
            auto dependent = (*dependency_info_it).second.first;
            auto & converter = (*dependency_info_it).second.second;
            m_value_store[dependent] = converter(store_value);
        }
    }

    m_parsing_completed = true;
}
