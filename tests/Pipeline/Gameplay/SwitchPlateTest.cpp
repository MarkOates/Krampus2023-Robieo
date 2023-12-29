
#include <gtest/gtest.h>

#include <Pipeline/Gameplay/SwitchPlate.hpp>


TEST(Pipeline_Gameplay_SwitchPlateTest, can_be_created_without_blowing_up)
{
   Pipeline::Gameplay::SwitchPlate switch_plate;
}


TEST(Pipeline_Gameplay_SwitchPlateTest, run__returns_the_expected_response)
{
   Pipeline::Gameplay::SwitchPlate switch_plate;
   std::string expected_string = "Hello World!";
   EXPECT_EQ(expected_string, switch_plate.run());
}


