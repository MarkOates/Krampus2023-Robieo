

#include <Pipeline/JSONLoaders/Pipeline/GameProgressAndStateInfo.hpp>


namespace Pipeline
{


void to_json(nlohmann::json& j, const GameProgressAndStateInfo& v)
{
   j = nlohmann::json{
      //{ "position", v.get_position() },
      { "delivered_package_identifiers", v.get_delivered_package_identifiers() },
   //j = nlohmann::json{
      //{ "main_character_display_name", v.get_main_character_display_name() },
      //{ "unlocked_achievement_identifiers", v.get_unlocked_achievement_identifiers() },
      //{ "unlocked_chapter_identifiers", v.get_unlocked_chapter_identifiers() },
      //{ "revealed_character_identifiers", v.get_revealed_character_identifiers() },
      //{ "last_entered_chapter_identifier", v.get_last_entered_chapter_identifier() },
   //};
   };
}

void from_json(const nlohmann::json& j, GameProgressAndStateInfo& v)
{
   //j.at("position").get_to(v.get_position_ref());
   j.at("delivered_package_identifiers").get_to(v.get_delivered_package_identifiers_ref());
}


} // namespace Pipeline



