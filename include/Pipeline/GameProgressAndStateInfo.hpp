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
      std::set<std::string> delivered_package_identifiers;

   protected:


   public:
      GameProgressAndStateInfo();
      virtual ~GameProgressAndStateInfo();

      void set_delivered_package_identifiers(std::set<std::string> delivered_package_identifiers);
      std::set<std::string> get_delivered_package_identifiers() const;
      std::set<std::string> &get_delivered_package_identifiers_ref();
      void mark_package_as_delivered(std::string delivered_package_identifier="[unset-delivered_package_identifier]");
      virtual std::string export_to_string() override;
      virtual void import_from_string(std::string data_string="[unset-data_string]") override;
   };
}



