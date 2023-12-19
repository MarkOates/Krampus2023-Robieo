
#include <gtest/gtest.h>

#include <Pipeline/GameProgressAndStateInfo.hpp>


TEST(Pipeline_GameProgressAndStateInfoTest, can_be_created_without_blowing_up)
{
   Pipeline::GameProgressAndStateInfo game_progress_and_state_info;
}


TEST(Pipeline_GameProgressAndStateInfoTest, TYPE__has_the_expected_value)
{
   EXPECT_STREQ(
     "Pipeline/GameProgressAndStateInfo",
     Pipeline::GameProgressAndStateInfo::TYPE
   );
}


TEST(Pipeline_GameProgressAndStateInfoTest, type__has_the_expected_value_matching_TYPE)
{
   Pipeline::GameProgressAndStateInfo game_progress_and_state_info;
   EXPECT_EQ(Pipeline::GameProgressAndStateInfo::TYPE, game_progress_and_state_info.get_type());
}


