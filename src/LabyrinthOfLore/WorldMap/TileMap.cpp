

#include <LabyrinthOfLore/WorldMap/TileMap.hpp>

#include <iostream>


namespace LabyrinthOfLore
{
   namespace WorldMap
   {
      TileMap::TileMap()
         : width(0.0f)
         , height(0.0f)
         , ceiling_height(100.0f)
         , tiles()
      {}


      TileMap::~TileMap()
      {
      }


      int TileMap::get_width()
      {
         return width;
      }


      int TileMap::get_height()
      {
         return height;
      }


      float TileMap::get_ceiling_height()
      {
         return ceiling_height;
      }


      int TileMap::infer_num_tiles()
      {
         return width * height;
      }


      bool TileMap::is_dimensionless()
      {
         return (width <= 0 || height <= 0);
      }


      Tile TileMap::get_tile(int tile_x, int tile_y)
      {
         if (tile_x < 0 || (tile_x >= width)) return -1;
         if (tile_y < 0 || (tile_y >= height)) return -1;

         return tiles[tile_x % width + tile_y * width];
      }


      void TileMap::set_ceiling_height(float ceiling_height)
      {
         if (ceiling_height < 1.0) throw std::runtime_error("Cannot set set_ceiling_height in TileMap to a value less than 1.0f.");
         this->ceiling_height = ceiling_height;
      }


      bool TileMap::set_tile(int tile_x, int tile_y, Tile value)
         // if the tile is set to a negative number, then the tiles[tile_index] will be set to that number, but
         // the image will be the bitmap at index 0
      {
         if (tile_x < 0 || (tile_x >= width)) return false;
         if (tile_y < 0 || (tile_y >= height)) return false;

         tiles[tile_x + tile_y * width] = value;

         return true;
      }


      std::pair<int, int> TileMap::get_coordinates_from_contiguous_number(int contiguous_tile_num)
      {
         int tile_x = contiguous_tile_num % width;
         int tile_y = contiguous_tile_num / width;

         return std::pair<int, int>(tile_x, tile_y);
      }


      void TileMap::resize(int w, int h, Tile tile_to_fill)
      {
         // set the width and height of our map
         width = w;
         height = h;

         // resize and clear the tiles
         tiles.assign(width * height, tile_to_fill);
      }

   } // namespace WorldMap
} // namespace LabyrinthOfLore



