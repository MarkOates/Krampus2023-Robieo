

#include <Pipeline/CSVToLevelLoader.hpp>

#include <AllegroFlare/CSVParser.hpp>
#include <AllegroFlare/Logger.hpp>
#include <AllegroFlare/UsefulPHP.hpp>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>


namespace Pipeline
{


CSVToLevelLoader::CSVToLevelLoader()
   : AllegroFlare::CSVParser()
   , csv_full_path("[unset-csv_full_path]")
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

int CSVToLevelLoader::toi(std::string value)
{
   if (value.empty()) return 0;
   if (value[0] == '+') value.erase(0, 1); // Pop front on the '+' sign
   return std::atoi(value.c_str());
}

float CSVToLevelLoader::tof(std::string value)
{
   if (value.empty()) return 0;
   if (value[0] == '+') value.erase(0, 1); // Pop front on the '+' sign
   return std::stof(value.c_str());
}

Pipeline::Gameplay::Level CSVToLevelLoader::find_level(std::string level_identifier)
{
   if (!(loaded))
   {
      std::stringstream error_message;
      error_message << "[CSVToLevelLoader::find_level]: error: guard \"loaded\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("CSVToLevelLoader::find_level: error: guard \"loaded\" not met");
   }
   if (!(level_exists(level_identifier)))
   {
      std::stringstream error_message;
      error_message << "[CSVToLevelLoader::find_level]: error: guard \"level_exists(level_identifier)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("CSVToLevelLoader::find_level: error: guard \"level_exists(level_identifier)\" not met");
   }
   return levels[level_identifier];
}

std::string CSVToLevelLoader::validate_key_and_return(std::map<std::string, std::string>* extracted_row, std::string key)
{
   if (extracted_row->count(key) == 0)
   {
      std::vector<std::string> valid_keys;
      for (const auto& pair : *extracted_row) valid_keys.push_back(pair.first);

      std::stringstream ss;
      ss << "[";
      for (auto &valid_key : valid_keys)
      {
         ss << "\"" << valid_key << "\", ";
      }
      ss << "]";

      AllegroFlare::Logger::throw_error(
         "Pipeline::CSVToLevelLoader::validate_key_and_return",
         "key \"" + key + "\" does not exist. The following keys are present: " + ss.str() + "."
      );
   }
   return extracted_row->operator[](key);
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
   // Obtain the content from the file and parse it to extractable data
   std::string content = AllegroFlare::php::file_get_contents(csv_full_path);
   if (content.empty()) throw std::runtime_error("empty file content");
   AllegroFlare::CSVParser csv_parser;
   csv_parser.set_raw_csv_content(content);
   csv_parser.parse();
   csv_parser.assemble_column_headers(3);

   // Load the parsed data to Level objects
   int first_physical_row = csv_parser.get_num_header_rows();
   for (std::map<std::string, std::string> &extracted_row : csv_parser.extract_all_rows())
   {
      // Pull out the variables
      std::string identifier =
         validate_key_and_return(&extracted_row, "identifier");
      std::string title =
         validate_key_and_return(&extracted_row, "title");
      std::string world_model_obj_filename =
         validate_key_and_return(&extracted_row, "world__model_obj_filename");
      std::string world_model_texture_filename =
         validate_key_and_return(&extracted_row, "world__model_texture_filename");

      std::string background_music_identifier =
         validate_key_and_return(&extracted_row, "background_music_identifier");
      std::string song_to_perform_identifier =
         validate_key_and_return(&extracted_row, "song_to_perform__identifier");
      float song_to_perform_duration_sec =
         tof(validate_key_and_return(&extracted_row, "song_to_perform__duration_sec"));

      float primary_light_spin =
         tof(validate_key_and_return(&extracted_row, "primary_light__spin"));
      float primary_light_tilt_time_of_day =
         tof(validate_key_and_return(&extracted_row, "primary_light__tilt_time_of_day"));



      // Pass along the variables to the result object
      Pipeline::Gameplay::Level level;
      level.set_title(title);

      level.set_world_model_obj_filename(world_model_obj_filename);
      level.set_world_model_texture_filename(world_model_texture_filename);




      {
      level.get_tile_maps_ref().push_back({});
      auto &tile_map = level.get_tile_maps_ref().back();

      std::string tile_map_tile_elevation_bitmap_filename =
         validate_key_and_return(&extracted_row, "tile_map_ground_floor__tile_elevation_bitmap_filename");
      std::string tile_map_tile_type_bitmap_filename =
         validate_key_and_return(&extracted_row, "tile_map_ground_floor__tile_type_bitmap_filename");
      std::cout << "tile_map_tile_elevation_bitmap_filename: " << tile_map_tile_elevation_bitmap_filename << std::endl;
      std::cout << "tile_map_tile_type_bitmap_filename: " << tile_map_tile_type_bitmap_filename << std::endl;

      int tile_map_origin_offset_x = toi(validate_key_and_return(&extracted_row, "tile_map_ground_floor__origin_offset__x"));
      int tile_map_origin_offset_y = toi(validate_key_and_return(&extracted_row, "tile_map_ground_floor__origin_offset__y"));
      float tile_map_ceiling_height = tof(validate_key_and_return(&extracted_row, "tile_map_ground_floor__ceiling_height"));
      float tile_map_groundlevel_height = tof(validate_key_and_return(&extracted_row, "tile_map_ground_floor__groundlevel_height"));
      float tile_map_floor_height = tof(validate_key_and_return(&extracted_row, "tile_map_ground_floor__floor_height"));

      tile_map.set_tile_elevation_bitmap_filename(tile_map_tile_elevation_bitmap_filename);
      tile_map.set_tile_type_bitmap_filename(tile_map_tile_type_bitmap_filename);
      tile_map.set_origin_offset({ (float)tile_map_origin_offset_x, (float)tile_map_origin_offset_y });
      tile_map.set_ceiling_height(tile_map_ceiling_height);
      tile_map.set_groundlevel_height(tile_map_groundlevel_height);
      tile_map.set_floor_height(tile_map_floor_height);
      }





      {
        //int num = 1;
      //level.get_tile_maps_ref().push_back({});
      //auto &tile_map = level.get_tile_maps_ref()[0];

      std::string tile_map_tile_elevation_bitmap_filename =
         validate_key_and_return(&extracted_row, "tile_map_floor2__tile_elevation_bitmap_filename");
      std::string tile_map_tile_type_bitmap_filename =
         validate_key_and_return(&extracted_row, "tile_map_floor2__tile_type_bitmap_filename");

      std::cout << "tile_map_tile_elevation_bitmap_filename: " << tile_map_tile_elevation_bitmap_filename << std::endl;
      std::cout << "tile_map_tile_type_bitmap_filename: " << tile_map_tile_type_bitmap_filename << std::endl;

         // TODO: Better validation
         if (!tile_map_tile_elevation_bitmap_filename.empty())
         {

         level.get_tile_maps_ref().push_back({});
         auto &tile_map = level.get_tile_maps_ref().back();
         int tile_map_origin_offset_x = toi(validate_key_and_return(&extracted_row, "tile_map_floor2__origin_offset__x"));
         int tile_map_origin_offset_y = toi(validate_key_and_return(&extracted_row, "tile_map_floor2__origin_offset__y"));
         float tile_map_ceiling_height = tof(validate_key_and_return(&extracted_row, "tile_map_floor2__ceiling_height"));
         float tile_map_groundlevel_height = tof(validate_key_and_return(&extracted_row, "tile_map_floor2__groundlevel_height"));
         float tile_map_floor_height = tof(validate_key_and_return(&extracted_row, "tile_map_floor2__floor_height"));

         tile_map.set_tile_elevation_bitmap_filename(tile_map_tile_elevation_bitmap_filename);
         tile_map.set_tile_type_bitmap_filename(tile_map_tile_type_bitmap_filename);
         tile_map.set_origin_offset({ (float)tile_map_origin_offset_x, (float)tile_map_origin_offset_y });
         tile_map.set_ceiling_height(tile_map_ceiling_height);
         tile_map.set_groundlevel_height(tile_map_groundlevel_height);
         tile_map.set_floor_height(tile_map_floor_height);
         }
      }




      {
        //int num = 1;
      //level.get_tile_maps_ref().push_back({});
      //auto &tile_map = level.get_tile_maps_ref()[0];

      std::string tile_map_tile_elevation_bitmap_filename =
         validate_key_and_return(&extracted_row, "tile_map_basement1__tile_elevation_bitmap_filename");
      std::string tile_map_tile_type_bitmap_filename =
         validate_key_and_return(&extracted_row, "tile_map_basement1__tile_type_bitmap_filename");

      std::cout << "tile_map_tile_elevation_bitmap_filename: " << tile_map_tile_elevation_bitmap_filename << std::endl;
      std::cout << "tile_map_tile_type_bitmap_filename: " << tile_map_tile_type_bitmap_filename << std::endl;

         // TODO: Better validation
         if (!tile_map_tile_elevation_bitmap_filename.empty())
         {

         level.get_tile_maps_ref().push_back({});
         auto &tile_map = level.get_tile_maps_ref().back();
         int tile_map_origin_offset_x = toi(validate_key_and_return(&extracted_row, "tile_map_basement1__origin_offset__x"));
         int tile_map_origin_offset_y = toi(validate_key_and_return(&extracted_row, "tile_map_basement1__origin_offset__y"));
         float tile_map_ceiling_height = tof(validate_key_and_return(&extracted_row, "tile_map_basement1__ceiling_height"));
         float tile_map_groundlevel_height = tof(validate_key_and_return(&extracted_row, "tile_map_basement1__groundlevel_height"));
         float tile_map_floor_height = tof(validate_key_and_return(&extracted_row, "tile_map_basement1__floor_height"));

         tile_map.set_tile_elevation_bitmap_filename(tile_map_tile_elevation_bitmap_filename);
         tile_map.set_tile_type_bitmap_filename(tile_map_tile_type_bitmap_filename);
         tile_map.set_origin_offset({ (float)tile_map_origin_offset_x, (float)tile_map_origin_offset_y });
         tile_map.set_ceiling_height(tile_map_ceiling_height);
         tile_map.set_groundlevel_height(tile_map_groundlevel_height);
         tile_map.set_floor_height(tile_map_floor_height);
         }
      }




      {
        //int num = 1;
      //level.get_tile_maps_ref().push_back({});
      //auto &tile_map = level.get_tile_maps_ref()[0];

      std::string tile_map_tile_elevation_bitmap_filename =
         validate_key_and_return(&extracted_row, "tile_map_basement2__tile_elevation_bitmap_filename");
      std::string tile_map_tile_type_bitmap_filename =
         validate_key_and_return(&extracted_row, "tile_map_basement2__tile_type_bitmap_filename");

      std::cout << "tile_map_tile_elevation_bitmap_filename: " << tile_map_tile_elevation_bitmap_filename << std::endl;
      std::cout << "tile_map_tile_type_bitmap_filename: " << tile_map_tile_type_bitmap_filename << std::endl;

         // TODO: Better validation
         if (!tile_map_tile_elevation_bitmap_filename.empty())
         {

         level.get_tile_maps_ref().push_back({});
         auto &tile_map = level.get_tile_maps_ref().back();
         int tile_map_origin_offset_x = toi(validate_key_and_return(&extracted_row, "tile_map_basement2__origin_offset__x"));
         int tile_map_origin_offset_y = toi(validate_key_and_return(&extracted_row, "tile_map_basement2__origin_offset__y"));
         float tile_map_ceiling_height = tof(validate_key_and_return(&extracted_row, "tile_map_basement2__ceiling_height"));
         float tile_map_groundlevel_height = tof(validate_key_and_return(&extracted_row, "tile_map_basement2__groundlevel_height"));
         float tile_map_floor_height = tof(validate_key_and_return(&extracted_row, "tile_map_basement2__floor_height"));

         tile_map.set_tile_elevation_bitmap_filename(tile_map_tile_elevation_bitmap_filename);
         tile_map.set_tile_type_bitmap_filename(tile_map_tile_type_bitmap_filename);
         tile_map.set_origin_offset({ (float)tile_map_origin_offset_x, (float)tile_map_origin_offset_y });
         tile_map.set_ceiling_height(tile_map_ceiling_height);
         tile_map.set_groundlevel_height(tile_map_groundlevel_height);
         tile_map.set_floor_height(tile_map_floor_height);
         }
      }





      //level.set_tile_elevation_bitmap_filename(tile_map_tile_elevation_bitmap_filename);
      //level.set_tile_map_tile_type_bitmap_filename(tile_map_tile_type_bitmap_filename);
      //level.set_tile_map_origin_offset({ (float)tile_map_origin_offset_x, (float)tile_map_origin_offset_y });
      //level.set_tile_map_ceiling_height(tile_map_ceiling_height);
      //level.set_tile_map_groundlevel_height(tile_map_groundlevel_height);
      //level.set_tile_map_floor_height(tile_map_floor_height);

      level.set_primary_light_spin(primary_light_spin);
      level.set_primary_light_tilt_time_of_day(primary_light_tilt_time_of_day);


      level.set_background_music_identifier(background_music_identifier);
      level.set_song_to_perform_identifier(song_to_perform_identifier);
      level.set_song_to_perform_duration_sec(song_to_perform_duration_sec);

      levels.insert({ identifier, level });
   }

   loaded = true;
   return;
}


} // namespace Pipeline


