

#include <Pipeline/Gameplay/Level.hpp>




namespace Pipeline
{
namespace Gameplay
{


Level::Level()
   : AllegroFlare::Levels::Base(Pipeline::Gameplay::Level::TYPE)
   , title("[unset-title]")
   , tile_map_origin_offset({})
   , tile_map_tile_elevation_bitmap_filename("[unset-tile_map_tile_elevation_bitmap_filename]")
   , tile_map_tile_type_bitmap_filename("[unset-tile_map_tile_type_bitmap_filename]")
{
}


Level::~Level()
{
}


void Level::set_title(std::string title)
{
   this->title = title;
}


void Level::set_tile_map_origin_offset(AllegroFlare::Vec2D tile_map_origin_offset)
{
   this->tile_map_origin_offset = tile_map_origin_offset;
}


void Level::set_tile_map_tile_elevation_bitmap_filename(std::string tile_map_tile_elevation_bitmap_filename)
{
   this->tile_map_tile_elevation_bitmap_filename = tile_map_tile_elevation_bitmap_filename;
}


void Level::set_tile_map_tile_type_bitmap_filename(std::string tile_map_tile_type_bitmap_filename)
{
   this->tile_map_tile_type_bitmap_filename = tile_map_tile_type_bitmap_filename;
}


std::string Level::get_title() const
{
   return title;
}


AllegroFlare::Vec2D Level::get_tile_map_origin_offset() const
{
   return tile_map_origin_offset;
}


std::string Level::get_tile_map_tile_elevation_bitmap_filename() const
{
   return tile_map_tile_elevation_bitmap_filename;
}


std::string Level::get_tile_map_tile_type_bitmap_filename() const
{
   return tile_map_tile_type_bitmap_filename;
}




} // namespace Gameplay
} // namespace Pipeline


