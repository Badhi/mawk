#include <gtest/gtest.h>

#include "MawkParser.h"

class TestMawkParser : public ::testing::Test
{
public:
    void SetUp(){

    }
    bool parse(std::string & str) { return p.parse_phrase(str); }
    MawkParser p;
};

std::vector<std::string> phrases = 
    {
        "BEGIN { }",
        "END { }", 
        "hello{ }", 
        "132{ }", 
        "/ab/{ }"
    };

TEST_F(TestMawkParser, setup)
{
    for(auto str : phrases)
        ASSERT_TRUE(parse(str)) << str;
}
