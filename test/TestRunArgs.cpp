#include <vector>
#include <fstream>

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

    EXPECT_EQ(' ', r[RunArgOptions::FIELD_SEPARATOR].as<char>());

}

TEST_F(TestRunArgs, default_args_with_field_sepearator)
{
    std::vector<const char*> args({"mawk", "-F", ","});

    RunArgs r(args.size(), args.data());

    r.parse_args();
    EXPECT_EQ(',', r[RunArgOptions::FIELD_SEPARATOR].as<char>());
}

TEST_F(TestRunArgs, default_args_multi_values)
{
    std::vector<const char*> args({"mawk", "-v", "a=5", "-v", "b=3", "-v", "c=8"});
    
    RunArgs r(args.size(), args.data());

    r.parse_args();

    auto a = r[RunArgOptions::VAR].as<std::vector<std::string>>();

    EXPECT_EQ(3u, a.size());

    EXPECT_EQ("a=5", a[0]);
    EXPECT_EQ("b=3", a[1]);
    EXPECT_EQ("c=8", a[2]);

}

TEST_F(TestRunArgs, manual_entry_add) 
{
    std::vector<const char*> args({"mawk", "-F", ",", "-f", "script.awk"});
    
    RunArgs r(args.size(), args.data());

    r("field_separator,F", po::value<char>()->default_value(' '), "Field separator", RunArgOptions::FIELD_SEPARATOR)
     ("file,f", po::value<std::string>(), "script file", RunArgOptions::SCRIPT_FILE);

    r.parse_args(false);

    EXPECT_EQ(',', r[RunArgOptions::FIELD_SEPARATOR].as<char>());
    EXPECT_EQ("script.awk", r[RunArgOptions::SCRIPT_FILE].as<std::string>());
}

TEST_F(TestRunArgs, dependent_flags_check)
{
    std::string sample_file_name = "script.awk";
    std::string script_body = "{ print $1}";

    std::ofstream sample_file(sample_file_name);
    sample_file << script_body;
    sample_file.close();

    std::vector<const char*> args({"mawk", "-f", sample_file_name.c_str()});

    RunArgs r(args.size(), args.data());

    r("file,f", po::value<std::string>(), "script file", RunArgOptions::SCRIPT_FILE, RunArgOptions::PHRASE, 
            [](const po::variable_value & val) {
                std::string script_name = val.as<std::string>();
                std::ifstream s(script_name);
                std::stringstream ss; 
                ss << s.rdbuf();
                boost::any phrase(ss.str());
                return po::variable_value(phrase, false);
        });

    r.parse_args(false);

    EXPECT_EQ(script_body, r[RunArgOptions::PHRASE].as<std::string>());
}

TEST_F(TestRunArgs, help_death)
{
    std::vector<const char*> args({"mawk", "--help"});

    RunArgs r(args.size(), args.data());

    EXPECT_DEATH(r.parse_args(), ".*");
}
