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

    r.parse_args();
}
