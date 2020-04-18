#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Logger.h"
#include "RunArgs.h"


int main(const int argc, const char ** argv)
{
    RunArgs r(argc, argv);
    r.parse_args(true);
}
