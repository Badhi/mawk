#include <gtest/gtest.h>

#include "MawkParser.h"


TEST(TestMawkParser, setup)
{
    MawkParser p;

    std::string phrase = "BEGIN{ }";

    p.parse_phrase(phrase);
}
