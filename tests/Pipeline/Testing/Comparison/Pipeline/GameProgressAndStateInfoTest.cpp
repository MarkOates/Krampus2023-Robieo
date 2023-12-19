
#include <gtest/gtest.h>

#include <Pipeline/Testing/Comparison/Pipeline/GameProgressAndStateInfo.hpp>


TEST(Pipeline_Testing_Comparison_Pipeline_GameProgressAndStateInfoTest,
   PrintTo__with_an_AllegroFlare_GameProgressAndStateInfo__will_output_as_expected)
{
   Pipeline::GameProgressAndStateInfo object;
   object.set_delivered_package_identifiers({"foo", "bar", "baz"});
   object.set_achievement_identifiers({"bonk", "boop"});

   Pipeline::GameProgressAndStateInfo other_object;
   other_object.set_delivered_package_identifiers({"foo", "bar", "baz"});
   other_object.set_achievement_identifiers({"bonk", "boop"});

   EXPECT_EQ(object, other_object);
}


TEST(FadeToWhite_Testing_Comparison_FadeToWhite_GameProgressAndStateInfoTest,
   equality_operator__works_with_google_test_EXPECT_EQ_statement)
{
   Pipeline::GameProgressAndStateInfo object;
   object.set_delivered_package_identifiers({"foo", "far", "faz"});
   object.set_achievement_identifiers({"bonk", "boop"});

   Pipeline::GameProgressAndStateInfo other_object;
   other_object.set_delivered_package_identifiers({"foo", "far", "faz"});
   other_object.set_achievement_identifiers({"bonk", "boop"});

   EXPECT_EQ(object, other_object);
}


TEST(Pipeline_Testing_Comparison_Pipeline_GameProgressAndStateInfoTest,
   inequality_operator__works_with_google_test_EXPECT_statement)
{
   Pipeline::GameProgressAndStateInfo object;
   object.set_delivered_package_identifiers({"foo", "far", "faz"});
   object.set_achievement_identifiers({"bonk", "boop"});

   Pipeline::GameProgressAndStateInfo other_object;
   other_object.set_delivered_package_identifiers({"booo", "barr", "bazz"});
   other_object.set_achievement_identifiers({"bonk", "boop"});

   EXPECT_NE(object, other_object);
}


