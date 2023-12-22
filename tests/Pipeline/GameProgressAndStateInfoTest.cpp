
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


TEST(Pipeline_GameProgressAndStateInfoTest,
   is_quest_completed__will_return_false_if_the_quest_identifier_is_not_in_the_list)
{
   Pipeline::GameProgressAndStateInfo game_progress_and_state_info;
   EXPECT_EQ(false, game_progress_and_state_info.is_quest_completed("a_quest_identifier_that_is_not_yet_finished"));
}


TEST(Pipeline_GameProgressAndStateInfoTest, is_quest_completed__will_return_true_if_the_quest_identifier_is_in_the_list)
{
   Pipeline::GameProgressAndStateInfo game_progress_and_state_info;
   game_progress_and_state_info.mark_quest_as_completed("a_completed_quest");
   EXPECT_EQ(true, game_progress_and_state_info.is_quest_completed("a_completed_quest"));
}


TEST(Pipeline_GameProgressAndStateInfoTest,
   count_num_items_in_inventory_with_identifier__will_return_the_number_of_a_particular_item_in_the_inventory)
{
   Pipeline::GameProgressAndStateInfo game_progress_and_state_info;
   game_progress_and_state_info.add_item_to_inventory("mushroom");
   game_progress_and_state_info.add_item_to_inventory("mushroom");
   game_progress_and_state_info.add_item_to_inventory("mushroom");
   game_progress_and_state_info.add_item_to_inventory("mushroom");
   EXPECT_EQ(4, game_progress_and_state_info.count_num_items_in_inventory_with_identifier("mushroom"));
}


