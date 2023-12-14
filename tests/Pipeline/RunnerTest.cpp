
#include <gtest/gtest.h>

#include <Pipeline/Runner.hpp>


TEST(Pipeline_RunnerTest, can_be_created_without_blowing_up)
{
   Pipeline::Runner runner;
}


TEST(Pipeline_RunnerTest, INTERACTIVE__run__will_work_as_expected)
{
   Pipeline::Runner runner;
   runner.run("test");
}


