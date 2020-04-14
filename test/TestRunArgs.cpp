#include <vector>

#include <RunArgs.h>
#include <gtest/gtest.h>

class TestRunArgs : public ::testing::Test
{
    
};

TEST_F(TestRunArgs, invaid_args)
{
    int argc = 0;
    const char ** argv = nullptr;
    EXPECT_DEATH(RunArgs r(argc, argv), ".*Assertion.*");
}

TEST_F(TestRunArgs, default_args)
{
    std::vector<const char*> args({"mawk"});

    RunArgs r(args.size(), args.data());

    r.parse_args(true);
}

TEST_F(TestRunArgs, default_args_default_values)
{
    std::vector<const char*> args({"mawk"});

    RunArgs r(args.size(), args.data());

    r.parse_args(true);

    EXPECT_EQ(' ', boost::any_cast<char>(r[RunArgOptions::FIELD_SEPARATOR]));

}

TEST_F(TestRunArgs, default_args_with_field_sepearator)
{
    std::vector<const char*> args({"mawk", "-F", "','"});

    RunArgs r(args.size(), args.data());

    r.parse_args(true);
}
