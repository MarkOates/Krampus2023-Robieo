#pragma once


#include <AllegroFlare/CSVParser.hpp>
#include <string>


namespace Pipeline
{
   class CSVToLevelLoader : public AllegroFlare::CSVParser
   {
   private:
      std::string csv_full_path;

   protected:


   public:
      CSVToLevelLoader();
      ~CSVToLevelLoader();

      void set_csv_full_path(std::string csv_full_path);
      std::string get_csv_full_path() const;
      void load();
   };
}



