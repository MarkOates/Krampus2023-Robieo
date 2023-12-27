

#include <Pipeline/Gameplay/LevelTileMap.hpp>




namespace Pipeline
{
namespace Gameplay
{


LevelTileMap::LevelTileMap()
   : tile_elevation_bitmap_filename("[unset-tile_elevation_bitmap_filename]")
   , tile_type_bitmap_filename("[unset-tile_type_bitmap_filename]")
   , origin_offset({})
   , ceiling_height(10.0f)
   , groundlevel_height(0.0f)
   , floor_height(-2.0f)
{
}


LevelTileMap::~LevelTileMap()
{
}


void LevelTileMap::set_tile_elevation_bitmap_filename(std::string tile_elevation_bitmap_filename)
{
   this->tile_elevation_bitmap_filename = tile_elevation_bitmap_filename;
}


void LevelTileMap::set_tile_type_bitmap_filename(std::string tile_type_bitmap_filename)
{
   this->tile_type_bitmap_filename = tile_type_bitmap_filename;
}


void LevelTileMap::set_origin_offset(AllegroFlare::Vec2D origin_offset)
{
   this->origin_offset = origin_offset;
}


void LevelTileMap::set_ceiling_height(float ceiling_height)
{
   this->ceiling_height = ceiling_height;
}


void LevelTileMap::set_groundlevel_height(float groundlevel_height)
{
   this->groundlevel_height = groundlevel_height;
}


void LevelTileMap::set_floor_height(float floor_height)
{
   this->floor_height = floor_height;
}


std::string LevelTileMap::get_tile_elevation_bitmap_filename() const
{
   return tile_elevation_bitmap_filename;
}


std::string LevelTileMap::get_tile_type_bitmap_filename() const
{
   return tile_type_bitmap_filename;
}


AllegroFlare::Vec2D LevelTileMap::get_origin_offset() const
{
   return origin_offset;
}


float LevelTileMap::get_ceiling_height() const
{
   return ceiling_height;
}


float LevelTileMap::get_groundlevel_height() const
{
   return groundlevel_height;
}


float LevelTileMap::get_floor_height() const
{
   return floor_height;
}




} // namespace Gameplay
} // namespace Pipeline


