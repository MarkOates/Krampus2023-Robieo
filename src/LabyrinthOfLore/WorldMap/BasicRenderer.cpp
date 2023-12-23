

#include <LabyrinthOfLore/WorldMap/BasicRenderer.hpp>

#include <AllegroFlare/Color.hpp>
#include <LabyrinthOfLore/WorldMap/Tile.hpp>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace LabyrinthOfLore
{
namespace WorldMap
{


BasicRenderer::BasicRenderer(LabyrinthOfLore::WorldMap::TileMap* tile_map, float tile_width, float tile_height)
   : tile_map(tile_map)
   , tile_width(tile_width)
   , tile_height(tile_height)
{
}


BasicRenderer::~BasicRenderer()
{
}


void BasicRenderer::set_tile_map(LabyrinthOfLore::WorldMap::TileMap* tile_map)
{
   this->tile_map = tile_map;
}


void BasicRenderer::set_tile_width(float tile_width)
{
   this->tile_width = tile_width;
}


void BasicRenderer::set_tile_height(float tile_height)
{
   this->tile_height = tile_height;
}


LabyrinthOfLore::WorldMap::TileMap* BasicRenderer::get_tile_map() const
{
   return tile_map;
}


float BasicRenderer::get_tile_width() const
{
   return tile_width;
}


float BasicRenderer::get_tile_height() const
{
   return tile_height;
}


void BasicRenderer::render()
{
   if (!(al_is_primitives_addon_initialized()))
   {
      std::stringstream error_message;
      error_message << "[BasicRenderer::render]: error: guard \"al_is_primitives_addon_initialized()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("BasicRenderer::render: error: guard \"al_is_primitives_addon_initialized()\" not met");
   }
   if (!(tile_map))
   {
      std::stringstream error_message;
      error_message << "[BasicRenderer::render]: error: guard \"tile_map\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("BasicRenderer::render: error: guard \"tile_map\" not met");
   }
   //int tile_width = 32;
   //int tile_height = 32;
   int inset = 2;

   float bottom_height = 0.0;
   float top_height = 10.0;
   ALLEGRO_COLOR tile_color = al_color_name("cyan");

   for (unsigned y=0; y<tile_map->get_height(); y++)
     for (unsigned x=0; x<tile_map->get_width(); x++)
      {
         LabyrinthOfLore::WorldMap::Tile tile = tile_map->get_tile(x, y);
         float height_float = (tile.get_height() / top_height * 0.5) + 0.5;

         ALLEGRO_COLOR this_tile_color = AllegroFlare::color::color(tile_color, height_float);

         al_draw_filled_rectangle(
            x * tile_width + inset,
            y * tile_height + inset,
            x * tile_width + tile_width - inset*2,
            y * tile_height + tile_height - inset*2,
            this_tile_color
         );
      }
   return;
}


} // namespace WorldMap
} // namespace LabyrinthOfLore


