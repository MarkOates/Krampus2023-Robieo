
#include <gtest/gtest.h>

#include <Pipeline/GameConfigurations/Main.hpp>


TEST(Pipeline_GameConfigurations_MainTest, can_be_created_without_blowing_up)
{
   Pipeline::GameConfigurations::Main main;
}


TEST(Pipeline_GameConfigurations_MainTest, load_level_by_identifier__will_not_blow_up)
{
   Pipeline::GameConfigurations::Main main;
   AllegroFlare::Levels::Base* level = main.load_level_by_identifier("forest_village_1");
   EXPECT_NE(nullptr, level);
}


