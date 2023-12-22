#pragma once


#include <AllegroFlare/GameProgressAndStateInfos/Base.hpp>
#include <set>
#include <string>


namespace Pipeline
{
   class GameProgressAndStateInfo : public AllegroFlare::GameProgressAndStateInfos::Base
   {
   public:
      static constexpr char* TYPE = (char*)"Pipeline/GameProgressAndStateInfo";

   private:
      std::set<std::string> achievement_identifiers;
      std::set<std::string> delivered_package_identifiers;
      std::multiset<std::string> inventory_item_identifiers;
      std::set<std::string> completed_quest_identifiers;

   protected:


   public:
      GameProgressAndStateInfo();
      virtual ~GameProgressAndStateInfo();

      void set_achievement_identifiers(std::set<std::string> achievement_identifiers);
      void set_delivered_package_identifiers(std::set<std::string> delivered_package_identifiers);
      void set_inventory_item_identifiers(std::multiset<std::string> inventory_item_identifiers);
      void set_completed_quest_identifiers(std::set<std::string> completed_quest_identifiers);
      std::set<std::string> get_achievement_identifiers() const;
      std::set<std::string> get_delivered_package_identifiers() const;
      std::multiset<std::string> get_inventory_item_identifiers() const;
      std::set<std::string> get_completed_quest_identifiers() const;
      std::set<std::string> &get_achievement_identifiers_ref();
      std::set<std::string> &get_delivered_package_identifiers_ref();
      std::multiset<std::string> &get_inventory_item_identifiers_ref();
      std::set<std::string> &get_completed_quest_identifiers_ref();
      void mark_achievement_as_unlocked(std::string achievement_identifier="[unset-achievement_identifier]");
      void mark_package_as_delivered(std::string delivered_package_identifier="[unset-delivered_package_identifier]");
      void mark_quest_as_completed(std::string completed_quest_identifier="[unset-completed_quest_identifier]");
      void add_item_to_inventory(std::string item_identifier="[unset-item_identifier]");
      bool is_quest_completed(std::string completed_quest_identifier="[unset-completed_quest_identifier]");
      virtual std::string export_to_string() override;
      virtual void import_from_string(std::string data_string="[unset-data_string]") override;
   };
}



