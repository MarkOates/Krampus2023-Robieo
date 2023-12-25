#pragma once


#include <AllegroFlare/DialogTree/NodeBank.hpp>
#include <AllegroFlare/GameConfigurations/Base.hpp>
#include <AllegroFlare/Levels/Base.hpp>
#include <Pipeline/CSVToLevelLoader.hpp>
#include <set>
#include <string>
#include <utility>
#include <vector>


namespace Pipeline
{
   namespace GameConfigurations
   {
      class Main : public AllegroFlare::GameConfigurations::Base
      {
      public:
         static constexpr char* TYPE = (char*)"Pipeline/GameConfigurations/Main";

      private:
         std::string data_folder;

      protected:


      public:
         Main(std::string data_folder="[unset-data_folder]");
         virtual ~Main();

         void set_data_folder(std::string data_folder);
         std::string get_data_folder() const;
         bool are_all_packages_delivered(std::set<std::string> delivered_package_identifiers={});
         virtual std::vector<std::pair<std::string, std::string>> build_level_list_for_level_select_screen_by_identifier(std::string identifier="[unset-identifier]") override;
         Pipeline::CSVToLevelLoader build_level_db();
         virtual AllegroFlare::DialogTree::NodeBank build_dialog_bank_by_identifier(std::string identifier="[identifier-discarded]") override;
         virtual AllegroFlare::Levels::Base* load_level_by_identifier(std::string identifier="[unset-identifier]") override;
      };
   }
}



