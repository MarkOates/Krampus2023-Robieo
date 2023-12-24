

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
   , tile_map_tile_elevation_bitmap_filename("[unset-tile_map_tile_elevation_bitmap_filename]")
   , tile_map_tile_type_bitmap_filename("[unset-tile_map_tile_type_bitmap_filename]")
   , tile_map_origin_offset({})
   , tile_map_ceiling_height(10.0f)
   , tile_map_groundlevel_height(0.0f)
   , tile_map_floor_height(-2.0f)
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


void Level::set_tile_map_tile_elevation_bitmap_filename(std::string tile_map_tile_elevation_bitmap_filename)
{
   this->tile_map_tile_elevation_bitmap_filename = tile_map_tile_elevation_bitmap_filename;
}


void Level::set_tile_map_tile_type_bitmap_filename(std::string tile_map_tile_type_bitmap_filename)
{
   this->tile_map_tile_type_bitmap_filename = tile_map_tile_type_bitmap_filename;
}


void Level::set_tile_map_origin_offset(AllegroFlare::Vec2D tile_map_origin_offset)
{
   this->tile_map_origin_offset = tile_map_origin_offset;
}


void Level::set_tile_map_ceiling_height(float tile_map_ceiling_height)
{
   this->tile_map_ceiling_height = tile_map_ceiling_height;
}


void Level::set_tile_map_groundlevel_height(float tile_map_groundlevel_height)
{
   this->tile_map_groundlevel_height = tile_map_groundlevel_height;
}


void Level::set_tile_map_floor_height(float tile_map_floor_height)
{
   this->tile_map_floor_height = tile_map_floor_height;
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


std::string Level::get_tile_map_tile_elevation_bitmap_filename() const
{
   return tile_map_tile_elevation_bitmap_filename;
}


std::string Level::get_tile_map_tile_type_bitmap_filename() const
{
   return tile_map_tile_type_bitmap_filename;
}


AllegroFlare::Vec2D Level::get_tile_map_origin_offset() const
{
   return tile_map_origin_offset;
}


float Level::get_tile_map_ceiling_height() const
{
   return tile_map_ceiling_height;
}


float Level::get_tile_map_groundlevel_height() const
{
   return tile_map_groundlevel_height;
}


float Level::get_tile_map_floor_height() const
{
   return tile_map_floor_height;
}


std::string Level::get_song_to_perform_identifier() const
{
   return song_to_perform_identifier;
}


float Level::get_song_to_perform_duration_sec() const
{
   return song_to_perform_duration_sec;
}




} // namespace Gameplay
} // namespace Pipeline


