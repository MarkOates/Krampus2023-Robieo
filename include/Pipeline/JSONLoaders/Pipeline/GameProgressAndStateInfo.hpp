#pragma once


#include <lib/nlohmann/json.hpp>
#include <Pipeline/GameProgressAndStateInfo.hpp>


namespace Pipeline
{
  void to_json(nlohmann::json& j, const GameProgressAndStateInfo& object);
  void from_json(const nlohmann::json& j, GameProgressAndStateInfo& object);
}


