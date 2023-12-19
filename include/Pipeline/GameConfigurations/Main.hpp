#pragma once


#include <AllegroFlare/DialogTree/NodeBank.hpp>
#include <AllegroFlare/GameConfigurations/Base.hpp>
#include <AllegroFlare/Levels/Base.hpp>
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

      protected:


      public:
         Main();
         virtual ~Main();

         bool are_all_packages_delivered(std::set<std::string> delivered_package_identifiers={});
         virtual std::vector<std::pair<std::string, std::string>> build_level_list_for_level_select_screen_by_identifier(std::string identifier="[unset-identifier]") override;
         virtual AllegroFlare::DialogTree::NodeBank build_dialog_bank_by_identifier(std::string identifier="[identifier-discarded]") override;
         virtual AllegroFlare::Levels::Base* load_level_by_identifier(std::string identifier="[unset-identifier]") override;
      };
   }
}



