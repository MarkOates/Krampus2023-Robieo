
#include <gtest/gtest.h>

#include <Pipeline/EntityAttributes.hpp>


TEST(Pipeline_EntityAttributesTest, can_be_created_without_blowing_up)
{
   Pipeline::EntityAttributes entity_attributes;
}


TEST(Pipeline_EntityAttributesTest, run__returns_the_expected_response)
{
   Pipeline::EntityAttributes entity_attributes;
   std::string expected_string = "Hello World!";
   EXPECT_EQ(expected_string, entity_attributes.run());
}


