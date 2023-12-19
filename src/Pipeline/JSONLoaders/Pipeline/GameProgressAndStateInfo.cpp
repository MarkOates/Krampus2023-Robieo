

#include <Pipeline/JSONLoaders/Pipeline/GameProgressAndStateInfo.hpp>


namespace Pipeline
{


void to_json(nlohmann::json& j, const GameProgressAndStateInfo& v)
{
   j = nlohmann::json{
      //{ "position", v.get_position() },
   };
}

void from_json(const nlohmann::json& j, GameProgressAndStateInfo& v)
{
   //j.at("position").get_to(v.get_position_ref());
}


} // namespace Pipeline



