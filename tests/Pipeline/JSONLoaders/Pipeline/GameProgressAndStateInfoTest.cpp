
#include <gtest/gtest.h>

#include <Pipeline/JSONLoaders/Pipeline/GameProgressAndStateInfo.hpp>
//#include <Pipeline/Testing/Comparison/Pipeline/GameProgressAndStateInfo.hpp>


TEST(Pipeline_JSONLoaders_Pipeline_GameProgressAndStateInfoTest,
   to_json__returns_the_object_as_json_with_the_expected_values)
{
   Pipeline::GameProgressAndStateInfo game_progress_and_state_info; //(4.0f);
   game_progress_and_state_info.set_delivered_package_identifiers({"package1", "package2"});
    
   nlohmann::json j = game_progress_and_state_info;

   std::string expected_values =
R"({
  "delivered_package_identifiers": [
    "package1",
    "package2"
  ]
})";

   std::string actual_values = j.dump(2);
   EXPECT_EQ(expected_values, actual_values);
}


TEST(Pipeline_JSONLoaders_Pipeline_GameProgressAndStateInfoTest,
   from_json__loads_json_data_into_the_object)
{
   //Pipeline::GameProgressAndStateInfo game_progress_and_state_info;

   //std::string json =
//R"({
  //"position": 1.5
//})";

   //nlohmann::json parsed_json = nlohmann::json::parse(json);
   //parsed_json.get_to(game_progress_and_state_info);

   //Pipeline::JSONLoaders::Pipeline::GameProgressAndStateInfo expected(1.5);

   //// TODO: add comparison
   //EXPECT_EQ(expected, game_progress_and_state_info);
}



