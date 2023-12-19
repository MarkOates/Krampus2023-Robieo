

#include <Pipeline/GameProgressAndStateInfo.hpp>

#include <Pipeline/JSONLoaders/Pipeline/GameProgressAndStateInfo.hpp>


namespace Pipeline
{


GameProgressAndStateInfo::GameProgressAndStateInfo()
   : AllegroFlare::GameProgressAndStateInfos::Base(Pipeline::GameProgressAndStateInfo::TYPE)
   , delivered_package_identifiers({})
{
}


GameProgressAndStateInfo::~GameProgressAndStateInfo()
{
}


void GameProgressAndStateInfo::set_delivered_package_identifiers(std::set<std::string> delivered_package_identifiers)
{
   this->delivered_package_identifiers = delivered_package_identifiers;
}


std::set<std::string> GameProgressAndStateInfo::get_delivered_package_identifiers() const
{
   return delivered_package_identifiers;
}


std::set<std::string> &GameProgressAndStateInfo::get_delivered_package_identifiers_ref()
{
   return delivered_package_identifiers;
}


void GameProgressAndStateInfo::mark_package_as_delivered(std::string delivered_package_identifier)
{
   delivered_package_identifiers.insert(delivered_package_identifier);
   return;
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


