

#include <Pipeline/GameConfigurations/Main.hpp>

#include <AllegroFlare/DialogTree/NodeBankFactory.hpp>
#include <AllegroFlare/Logger.hpp>
#include <Pipeline/Gameplay/Level.hpp>
#include <functional>
#include <map>
#include <set>
#include <sstream>


namespace Pipeline
{
namespace GameConfigurations
{


Main::Main()
   : AllegroFlare::GameConfigurations::Base(Pipeline::GameConfigurations::Main::TYPE)
{
}


Main::~Main()
{
}


bool Main::are_all_packages_delivered(std::set<std::string> delivered_package_identifiers)
{
   std::vector<std::pair<std::string, std::string>> all_levels =
      build_level_list_for_level_select_screen_by_identifier();

   // Build a list of all packages (from the list of levels for the level select screen)
   std::set<std::string> list_of_all_packages;
   for (auto &level : all_levels)
   {
      std::string level_identifier = level.second;
      list_of_all_packages.insert(level_identifier);
   }


   for (const auto& element : list_of_all_packages) {
      if (delivered_package_identifiers.find(element) == delivered_package_identifiers.end()) {
         // Element from set1 is not present in set2
         return false;
      }
   }
   return true;
    //All elements from set1 are present in set2
   //return true;


   // Validate the list is sound (has the same number of elements as the level select screen)
   if (list_of_all_packages.size() != all_levels.size())
   {
      // There are non-unique level identifiers. That's unexpected for this game
      AllegroFlare::Logger::throw_error(
         "Pipeline::GameConfigurations::Main::are_all_packages_delivered",
         "When attempting to build the list_of_all_packages, it was not the same size as the levels for the level "
            "select screen, indicating there are non-unique level identifiers in the list. That's unexpected for "
            "this game."
      );
   }

   // Return if the delivered packages an list of all packages are the same
   // TODO: Test this
   return list_of_all_packages == delivered_package_identifiers;
}

std::vector<std::pair<std::string, std::string>> Main::build_level_list_for_level_select_screen_by_identifier(std::string identifier)
{
   std::vector<std::pair<std::string, std::string>> result = {
      { "World 2", "world-2-06" },
      { "World 3", "world-3-09" },
      { "World 4", "level-4-05" },
      //{ "World 2", "world-2-01" },
      //{ "Forest Village 1", "forest_village_1" },
      //{ "Forest Village 2", "forest_village_2" },
      //{ "Forest", "forest_1" },
      //{ "Crystal Cave", "crystal_cave_1" },
      //{ "Desert Town", "desert_town_3" },
      //{ "Town 2", "town_2" },
      //{ "Cave 1", "cave_1" },
      //{ "Town 1", "town_1" },
   };
   return result;
}

AllegroFlare::DialogTree::NodeBank Main::build_dialog_bank_by_identifier(std::string identifier)
{
   // TODO: Test this contains the expected nodes
   AllegroFlare::DialogTree::NodeBank result_node_bank;

   // TODO: Consider joining the system nodes outside of the LevelFactory
   AllegroFlare::DialogTree::NodeBank system_node_bank =
      AllegroFlare::DialogTree::NodeBankFactory::build_common_system_dialogs_node_bank();
   result_node_bank.merge(&system_node_bank);

   return result_node_bank;
}

AllegroFlare::Levels::Base* Main::load_level_by_identifier(std::string identifier)
{
   // TODO: Replace void* with a better type (once the design is clear)
   Pipeline::Gameplay::Level *result = new Pipeline::Gameplay::Level();

   std::map<std::string, std::function<void()>> items_map = {
      { "forest_village_1", [result](){
         //result->set_background_image_identifier("maps/forest-village-1.png");
      }},
      { "forest_village_2", [result](){
         //result->set_background_image_identifier("maps/forest-village-2.png");
      }},
      { "forest_1", [result](){
         //result->set_background_image_identifier("maps/forest-1.png");
      }},
      { "crystal_cave_1", [result](){
         //result->set_background_image_identifier("maps/crystal-cave-1.png");
      }},
      { "desert_town_3", [result](){
         //result->set_background_image_identifier("maps/desert-town-3.png");
      }},
      { "town_2", [result](){
         //result->set_background_image_identifier("maps/town-2.png");
      }},
      { "cave_1", [result](){
         //result->set_background_image_identifier("maps/rpg-fit-backgrounds-ex-101.png");
      }},
      { "town_1", [result](){
         //result->set_background_image_identifier("maps/rpg-fit-backgrounds-x2-01.png");
      }},
   };

   // locate and call the function to handle the item
   if (items_map.count(identifier) == 0)
   {
      bool item_handled = false;
      //if (unrecognized_key_callback_func)
      //{
         //item_handled = unrecognized_key_callback_func();
      //}

      if (!item_handled)
      {
         // item not found
         std::stringstream error_message;
         error_message << "[Pipeline::GameConfigurations::Main::load_level]: error: Cannot load the item with the identifier \""
                       << identifier << "\", it does not exist.";
         throw std::runtime_error(error_message.str());
      }
   }
   else
   {
      // call the item
      items_map[identifier]();
   }

   return result;
}


} // namespace GameConfigurations
} // namespace Pipeline


