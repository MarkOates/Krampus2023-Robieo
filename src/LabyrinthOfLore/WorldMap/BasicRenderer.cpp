

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


BasicRenderer::BasicRenderer(LabyrinthOfLore::WorldMap::TileMap tile_map)
   : tile_map(tile_map)
{
}


BasicRenderer::~BasicRenderer()
{
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
   int tile_width = 32;
   int tile_height = 32;

   float bottom_height = 0.0;
   float top_height = 10.0;
   ALLEGRO_COLOR tile_color = al_color_name("chartreuse");

   for (unsigned y=0; y<tile_map.get_height(); y++)
     for (unsigned x=0; x<tile_map.get_width(); x++)
      {
         LabyrinthOfLore::WorldMap::Tile tile = tile_map.get_tile(x, y);
         float height_float = tile.get_height() / top_height;

         ALLEGRO_COLOR this_tile_color = AllegroFlare::color::color(tile_color, height_float);

         al_draw_filled_rectangle(
            x * tile_width,
            y * tile_height,
            x * tile_width + tile_width,
            y * tile_height + tile_height,
            this_tile_color
         );
      }
   return;
}


} // namespace WorldMap
} // namespace LabyrinthOfLore


