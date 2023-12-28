
#include <gtest/gtest.h>

#include <Pipeline/Gameplay/LevelCameraZone.hpp>


TEST(Pipeline_Gameplay_LevelCameraZoneTest, can_be_created_without_blowing_up)
{
   Pipeline::Gameplay::LevelCameraZone level_camera_zone;
}


TEST(Pipeline_Gameplay_LevelCameraZoneTest, run__returns_the_expected_response)
{
   Pipeline::Gameplay::LevelCameraZone level_camera_zone;
   std::string expected_string = "Hello World!";
   EXPECT_EQ(expected_string, level_camera_zone.run());
}


