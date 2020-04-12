#include <iostream>

#include "RunArgs.h"

RunArgs::RunArgs(const int argc, const char ** argv) noexcept : 
                m_desc("Allowed Options"), 
                m_name_mapping(static_cast<unsigned int>(RunArgOptions::END)),
                m_argc(argc), m_argv(argv)
{
}



void RunArgs::parse_args()
{
    po::store(po::parse_command_line(m_argc, m_argv, m_desc), m_vm);
    po::notify(m_vm);

    if(m_vm.count("help")){
        std::cerr << m_desc << "\n"; 
        exit(1); 
    }
}
