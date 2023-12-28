

#include <Pipeline/Gameplay/Level.hpp>




namespace Pipeline
{
namespace Gameplay
{


Level::Level()
   : AllegroFlare::Levels::Base(Pipeline::Gameplay::Level::TYPE)
   , title("[unset-title]")
   , world_model_obj_filename("[unset-world_model_obj_filename]")
   , world_model_texture_filename("[unset-world_model_texture_filename]")
   , tile_maps({})
   , background_music_identifier("[unset-background_music_identifier]")
   , primary_light_spin(-1.0f)
   , primary_light_tilt_time_of_day(0.15f)
   , song_to_perform_identifier("[unset-song_to_perform_identifier]")
   , song_to_perform_duration_sec(6.0f)
{
}


Level::~Level()
{
}


void Level::set_title(std::string title)
{
   this->title = title;
}


void Level::set_world_model_obj_filename(std::string world_model_obj_filename)
{
   this->world_model_obj_filename = world_model_obj_filename;
}


void Level::set_world_model_texture_filename(std::string world_model_texture_filename)
{
   this->world_model_texture_filename = world_model_texture_filename;
}


void Level::set_tile_maps(std::vector<Pipeline::Gameplay::LevelTileMap> tile_maps)
{
   this->tile_maps = tile_maps;
}


void Level::set_background_music_identifier(std::string background_music_identifier)
{
   this->background_music_identifier = background_music_identifier;
}


void Level::set_primary_light_spin(float primary_light_spin)
{
   this->primary_light_spin = primary_light_spin;
}


void Level::set_primary_light_tilt_time_of_day(float primary_light_tilt_time_of_day)
{
   this->primary_light_tilt_time_of_day = primary_light_tilt_time_of_day;
}


void Level::set_song_to_perform_identifier(std::string song_to_perform_identifier)
{
   this->song_to_perform_identifier = song_to_perform_identifier;
}


void Level::set_song_to_perform_duration_sec(float song_to_perform_duration_sec)
{
   this->song_to_perform_duration_sec = song_to_perform_duration_sec;
}


std::string Level::get_title() const
{
   return title;
}


std::string Level::get_world_model_obj_filename() const
{
   return world_model_obj_filename;
}


std::string Level::get_world_model_texture_filename() const
{
   return world_model_texture_filename;
}


std::vector<Pipeline::Gameplay::LevelTileMap> Level::get_tile_maps() const
{
   return tile_maps;
}


std::string Level::get_background_music_identifier() const
{
   return background_music_identifier;
}


float Level::get_primary_light_spin() const
{
   return primary_light_spin;
}


float Level::get_primary_light_tilt_time_of_day() const
{
   return primary_light_tilt_time_of_day;
}


std::string Level::get_song_to_perform_identifier() const
{
   return song_to_perform_identifier;
}


float Level::get_song_to_perform_duration_sec() const
{
   return song_to_perform_duration_sec;
}


std::vector<Pipeline::Gameplay::LevelTileMap> &Level::get_tile_maps_ref()
{
   return tile_maps;
}




} // namespace Gameplay
} // namespace Pipeline


