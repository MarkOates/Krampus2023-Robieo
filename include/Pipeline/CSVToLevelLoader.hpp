#pragma once


#include <AllegroFlare/CSVParser.hpp>
#include <Pipeline/Gameplay/Level.hpp>
#include <map>
#include <string>


namespace Pipeline
{
   class CSVToLevelLoader : public AllegroFlare::CSVParser
   {
   private:
      std::string csv_full_path;
      std::map<std::string, Pipeline::Gameplay::Level> levels;
      bool loaded;

   protected:


   public:
      CSVToLevelLoader();
      ~CSVToLevelLoader();

      void set_csv_full_path(std::string csv_full_path);
      std::string get_csv_full_path() const;
      std::map<std::string, Pipeline::Gameplay::Level> get_levels();
      bool level_exists(std::string level_identifier="[unset-level_identifier]");
      Pipeline::Gameplay::Level find_level(std::string level_identifier="[unset-level_identifier]");
      static std::string validate_key_and_return(std::map<std::string, std::string>* extracted_row=nullptr, std::string key="[unset-key]");
      void load();
   };
}



