
#include <gtest/gtest.h>

#include <Pipeline/SmoothCamera.hpp>


TEST(Pipeline_SmoothCameraTest, can_be_created_without_blowing_up)
{
   Pipeline::SmoothCamera smooth_camera;
}


TEST(Pipeline_SmoothCameraTest, update__sets_the_current_camera_position)
{
   Pipeline::SmoothCamera smooth_camera;
   //std::string expected_string = "Hello World!";
   //EXPECT_EQ(expected_string, smooth_camera.run());
}


