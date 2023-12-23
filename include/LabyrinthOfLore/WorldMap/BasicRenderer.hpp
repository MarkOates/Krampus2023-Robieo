#pragma once


#include <LabyrinthOfLore/WorldMap/TileMap.hpp>


namespace LabyrinthOfLore
{
   namespace WorldMap
   {
      class BasicRenderer
      {
      private:
         LabyrinthOfLore::WorldMap::TileMap tile_map;

      protected:


      public:
         BasicRenderer(LabyrinthOfLore::WorldMap::TileMap tile_map={});
         ~BasicRenderer();

         void render();
      };
   }
}



