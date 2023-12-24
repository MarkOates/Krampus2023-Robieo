#pragma once


#include <LabyrinthOfLore/WorldMap/TileMap.hpp>
#include <string>


namespace LabyrinthOfLore
{
   namespace WorldMap
   {
      class MultiBitmapFilenameToWorldBuilder
      {
      private:
         std::string elevation_bitmap_source_filename;
         std::string tile_type_bitmap_source_filename;
         float ceiling_height;
         float ground_height;
         float floor_height;

      protected:


      public:
         MultiBitmapFilenameToWorldBuilder(std::string elevation_bitmap_source_filename="data/bitmaps/the_underworld-elevation.png", std::string tile_type_bitmap_source_filename="data/bitmaps/the_underworld-type.png", float ceiling_height=2.0f, float ground_height=1.0f, float floor_height=0.0f);
         ~MultiBitmapFilenameToWorldBuilder();

         std::string get_elevation_bitmap_source_filename() const;
         std::string get_tile_type_bitmap_source_filename() const;
         float get_ceiling_height() const;
         float get_ground_height() const;
         float get_floor_height() const;
         LabyrinthOfLore::WorldMap::TileMap build();
      };
   }
}



