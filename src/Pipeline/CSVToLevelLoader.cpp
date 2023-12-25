

#include <Pipeline/CSVToLevelLoader.hpp>

#include <AllegroFlare/CSVParser.hpp>
#include <AllegroFlare/UsefulPHP.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace Pipeline
{


CSVToLevelLoader::CSVToLevelLoader()
   : AllegroFlare::CSVParser()
   , csv_full_path("/Users/markoates/Repos/SurviveTheCity/bin/data/levels/universe.csv")
   , levels({})
   , loaded(false)
{
}


CSVToLevelLoader::~CSVToLevelLoader()
{
}


void CSVToLevelLoader::set_csv_full_path(std::string csv_full_path)
{
   this->csv_full_path = csv_full_path;
}


std::string CSVToLevelLoader::get_csv_full_path() const
{
   return csv_full_path;
}


std::map<std::string, Pipeline::Gameplay::Level> CSVToLevelLoader::get_levels()
{
   if (!(loaded))
   {
      std::stringstream error_message;
      error_message << "[CSVToLevelLoader::get_levels]: error: guard \"loaded\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("CSVToLevelLoader::get_levels: error: guard \"loaded\" not met");
   }
   return levels;
}

bool CSVToLevelLoader::level_exists(std::string level_identifier)
{
   if (!(loaded))
   {
      std::stringstream error_message;
      error_message << "[CSVToLevelLoader::level_exists]: error: guard \"loaded\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("CSVToLevelLoader::level_exists: error: guard \"loaded\" not met");
   }
   return (levels.find(level_identifier) != levels.end());
}

Pipeline::Gameplay::Level CSVToLevelLoader::get_copy_of_level(std::string level_identifier)
{
   if (!(loaded))
   {
      std::stringstream error_message;
      error_message << "[CSVToLevelLoader::get_copy_of_level]: error: guard \"loaded\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("CSVToLevelLoader::get_copy_of_level: error: guard \"loaded\" not met");
   }
   if (!(level_exists(level_identifier)))
   {
      std::stringstream error_message;
      error_message << "[CSVToLevelLoader::get_copy_of_level]: error: guard \"level_exists(level_identifier)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("CSVToLevelLoader::get_copy_of_level: error: guard \"level_exists(level_identifier)\" not met");
   }
   return levels[level_identifier];
}

void CSVToLevelLoader::load()
{
   if (!((!loaded)))
   {
      std::stringstream error_message;
      error_message << "[CSVToLevelLoader::load]: error: guard \"(!loaded)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("CSVToLevelLoader::load: error: guard \"(!loaded)\" not met");
   }
   //std::string HARD_CODED_DB_FILE = "/Users/markoates/Repos/SurviveTheCity/bin/data/levels/universe.csv";
   //std::filesystem::exists(HARD_CODED_DB_FILE);
   std::string content = AllegroFlare::php::file_get_contents(csv_full_path);
   if (content.empty()) throw std::runtime_error("empty file content");
   AllegroFlare::CSVParser csv_parser;
   csv_parser.set_raw_csv_content(content);
   csv_parser.parse();
   csv_parser.assemble_column_headers(2);
   loaded = true;
   return;
}


} // namespace Pipeline


