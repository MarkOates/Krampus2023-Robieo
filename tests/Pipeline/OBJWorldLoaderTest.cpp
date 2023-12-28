
#include <gtest/gtest.h>

#include <Pipeline/OBJWorldLoader.hpp>


TEST(Pipeline_OBJWorldLoaderTest, can_be_created_without_blowing_up)
{
   Pipeline::OBJWorldLoader objworld_loader;
}


TEST(Pipeline_OBJWorldLoaderTest, load__will_not_blow_up)
{
   Pipeline::OBJWorldLoader objworld_loader;
   //std::string expected_string = "Hello World!";
   //EXPECT_EQ(expected_string, objworld_loader.run());
}


