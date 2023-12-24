#pragma once


#include <LabyrinthOfLore/WorldMap/TileMap.hpp>
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>


namespace LabyrinthOfLore
{
   namespace WorldMap
   {
      class BitmapTileMapLoader
      {
      private:
         ALLEGRO_BITMAP* source_bitmap;

      protected:


      public:
         BitmapTileMapLoader(ALLEGRO_BITMAP* source_bitmap=nullptr);
         ~BitmapTileMapLoader();

         bool colors_are_equal(ALLEGRO_COLOR c1={}, ALLEGRO_COLOR c2={});
         int get_index_column_x();
         int infer_tile_map_width();
         int infer_tile_map_height();
         ALLEGRO_COLOR pick_color(int x=0, int y=0);
         ALLEGRO_COLOR pick_index_null_color();
         ALLEGRO_COLOR pick_index_top_color();
         ALLEGRO_COLOR pick_index_ground_color();
         ALLEGRO_COLOR pick_index_gp_color();
         ALLEGRO_COLOR pick_index_gpp_color();
         ALLEGRO_COLOR pick_index_gppp_color();
         ALLEGRO_COLOR pick_index_g1_color();
         ALLEGRO_COLOR pick_index_gm_color();
         ALLEGRO_COLOR pick_index_gmm_color();
         ALLEGRO_COLOR pick_index_gmmm_color();
         ALLEGRO_COLOR pick_index_gm1_color();
         ALLEGRO_COLOR pick_index_g1p_color();
         ALLEGRO_COLOR pick_index_g1pp_color();
         ALLEGRO_COLOR pick_index_g1ppp_color();
         ALLEGRO_COLOR pick_index_g2_color();
         bool validate();
         LabyrinthOfLore::WorldMap::TileMap load(float ceiling_height=4.0f, float ground_height=2.0f);
      };
   }
}



