

#include <Pipeline/CSVToLevelLoader.hpp>

#include <AllegroFlare/CSVParser.hpp>
#include <AllegroFlare/UsefulPHP.hpp>


namespace Pipeline
{


CSVToLevelLoader::CSVToLevelLoader()
   : AllegroFlare::CSVParser()
   , csv_full_path("/Users/markoates/Repos/SurviveTheCity/bin/data/levels/universe.csv")
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


void CSVToLevelLoader::load()
{
   //std::string HARD_CODED_DB_FILE = "/Users/markoates/Repos/SurviveTheCity/bin/data/levels/universe.csv";
   //std::filesystem::exists(HARD_CODED_DB_FILE);
   std::string content = AllegroFlare::php::file_get_contents(csv_full_path);
   if (content.empty()) throw std::runtime_error("empty file content");
   AllegroFlare::CSVParser csv_parser;
   csv_parser.set_raw_csv_content(content);
   csv_parser.parse();
   csv_parser.assemble_column_headers(2);
   return;
}


} // namespace Pipeline


