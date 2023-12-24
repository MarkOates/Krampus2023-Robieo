#pragma once


#include <LabyrinthOfLore/WorldMap/TileMap.hpp>
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>


namespace LabyrinthOfLore
{
   namespace WorldMap
   {
      class MultiBitmapTileMapLoader
      {
      private:
         LabyrinthOfLore::WorldMap::TileMap* tile_map;
         ALLEGRO_BITMAP* source_bitmap;

      protected:


      public:
         MultiBitmapTileMapLoader(LabyrinthOfLore::WorldMap::TileMap* tile_map=nullptr, ALLEGRO_BITMAP* source_bitmap=nullptr);
         ~MultiBitmapTileMapLoader();

         bool colors_are_equal(ALLEGRO_COLOR c1={}, ALLEGRO_COLOR c2={});
         int get_index_column_x();
         int infer_tile_map_width();
         int infer_tile_map_height();
         ALLEGRO_COLOR pick_color(int x=0, int y=0);
         ALLEGRO_COLOR pick_index_null_color();
         ALLEGRO_COLOR pick_door_color();
         ALLEGRO_COLOR pick_water_color();
         ALLEGRO_COLOR pick_lava_color();
         ALLEGRO_COLOR pick_glow_water_color();
         bool validate();
         void load_and_process();
      };
   }
}



