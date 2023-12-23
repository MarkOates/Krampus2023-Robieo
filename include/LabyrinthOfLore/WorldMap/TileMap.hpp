#pragma once


#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include <LabyrinthOfLore/WorldMap/Tile.hpp>


namespace LabyrinthOfLore
{
   namespace WorldMap
   {
      class TileMap
      {
      private:
         int width;
         int height;
         float ceiling_height;
         std::vector<Tile> tiles;

      public:

         TileMap();
         ~TileMap();

         int get_width();
         int get_height();
         float get_ceiling_height();
         int infer_num_tiles();
         bool is_dimensionless();

         Tile get_tile(int tile_x, int tile_y);
         void set_ceiling_height(float height);
         bool set_tile(int tile_x, int tile_y, Tile value);
         std::pair<int, int> get_coordinates_from_contiguous_number(int contiguous_tile_num);

         void resize(int w, int h, Tile tile_to_fill=Tile());
      };
   }
}


