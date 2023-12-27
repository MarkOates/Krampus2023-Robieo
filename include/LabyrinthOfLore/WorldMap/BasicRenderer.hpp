#pragma once


#include <LabyrinthOfLore/WorldMap/TileMap.hpp>


namespace LabyrinthOfLore
{
   namespace WorldMap
   {
      class BasicRenderer
      {
      private:
         LabyrinthOfLore::WorldMap::TileMap* tile_map;
         float tile_width;
         float tile_height;
         float groundlevel_height;

      protected:


      public:
         BasicRenderer(LabyrinthOfLore::WorldMap::TileMap* tile_map={}, float tile_width=32, float tile_height=32, float groundlevel_height=0.0f);
         ~BasicRenderer();

         void set_tile_map(LabyrinthOfLore::WorldMap::TileMap* tile_map);
         void set_tile_width(float tile_width);
         void set_tile_height(float tile_height);
         void set_groundlevel_height(float groundlevel_height);
         LabyrinthOfLore::WorldMap::TileMap* get_tile_map() const;
         float get_tile_width() const;
         float get_tile_height() const;
         float get_groundlevel_height() const;
         void render();
      };
   }
}



