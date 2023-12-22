

#include <Pipeline/GameProgressAndStateInfo.hpp>

#include <Pipeline/JSONLoaders/Pipeline/GameProgressAndStateInfo.hpp>


namespace Pipeline
{


GameProgressAndStateInfo::GameProgressAndStateInfo()
   : AllegroFlare::GameProgressAndStateInfos::Base(Pipeline::GameProgressAndStateInfo::TYPE)
   , achievement_identifiers({})
   , delivered_package_identifiers({})
   , inventory_item_identifiers({})
   , completed_quest_identifiers({})
{
}


GameProgressAndStateInfo::~GameProgressAndStateInfo()
{
}


void GameProgressAndStateInfo::set_achievement_identifiers(std::set<std::string> achievement_identifiers)
{
   this->achievement_identifiers = achievement_identifiers;
}


void GameProgressAndStateInfo::set_delivered_package_identifiers(std::set<std::string> delivered_package_identifiers)
{
   this->delivered_package_identifiers = delivered_package_identifiers;
}


void GameProgressAndStateInfo::set_inventory_item_identifiers(std::multiset<std::string> inventory_item_identifiers)
{
   this->inventory_item_identifiers = inventory_item_identifiers;
}


void GameProgressAndStateInfo::set_completed_quest_identifiers(std::set<std::string> completed_quest_identifiers)
{
   this->completed_quest_identifiers = completed_quest_identifiers;
}


std::set<std::string> GameProgressAndStateInfo::get_achievement_identifiers() const
{
   return achievement_identifiers;
}


std::set<std::string> GameProgressAndStateInfo::get_delivered_package_identifiers() const
{
   return delivered_package_identifiers;
}


std::multiset<std::string> GameProgressAndStateInfo::get_inventory_item_identifiers() const
{
   return inventory_item_identifiers;
}


std::set<std::string> GameProgressAndStateInfo::get_completed_quest_identifiers() const
{
   return completed_quest_identifiers;
}


std::set<std::string> &GameProgressAndStateInfo::get_achievement_identifiers_ref()
{
   return achievement_identifiers;
}


std::set<std::string> &GameProgressAndStateInfo::get_delivered_package_identifiers_ref()
{
   return delivered_package_identifiers;
}


std::multiset<std::string> &GameProgressAndStateInfo::get_inventory_item_identifiers_ref()
{
   return inventory_item_identifiers;
}


std::set<std::string> &GameProgressAndStateInfo::get_completed_quest_identifiers_ref()
{
   return completed_quest_identifiers;
}


void GameProgressAndStateInfo::mark_achievement_as_unlocked(std::string achievement_identifier)
{
   achievement_identifiers.insert(achievement_identifier);
   return;
}

void GameProgressAndStateInfo::mark_package_as_delivered(std::string delivered_package_identifier)
{
   delivered_package_identifiers.insert(delivered_package_identifier);
   return;
}

void GameProgressAndStateInfo::mark_quest_as_completed(std::string completed_quest_identifier)
{
   completed_quest_identifiers.insert(completed_quest_identifier);
   return;
}

void GameProgressAndStateInfo::add_item_to_inventory(std::string item_identifier)
{
   inventory_item_identifiers.insert(item_identifier);
   return;
}

bool GameProgressAndStateInfo::is_quest_completed(std::string completed_quest_identifier)
{
   return (completed_quest_identifiers.find(completed_quest_identifier) != completed_quest_identifiers.end());
}

std::string GameProgressAndStateInfo::export_to_string()
{
   nlohmann::json j = *this;
   std::string string_dump = j.dump(2);
   return string_dump;
}

void GameProgressAndStateInfo::import_from_string(std::string data_string)
{
   nlohmann::json parsed_json = nlohmann::json::parse(data_string);
   parsed_json.get_to(*this);
}


} // namespace Pipeline


