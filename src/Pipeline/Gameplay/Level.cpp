

#include <Pipeline/Gameplay/Level.hpp>




namespace Pipeline
{
namespace Gameplay
{


Level::Level()
   : AllegroFlare::Levels::Base(Pipeline::Gameplay::Level::TYPE)
   , title("[unset-title]")
   , tile_map_origin_offset({})
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


std::string Level::get_title() const
{
   return title;
}


AllegroFlare::Vec2D Level::get_tile_map_origin_offset() const
{
   return tile_map_origin_offset;
}




} // namespace Gameplay
} // namespace Pipeline


