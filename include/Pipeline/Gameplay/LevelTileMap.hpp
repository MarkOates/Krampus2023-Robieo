#pragma once


#include <AllegroFlare/Vec2D.hpp>
#include <string>


namespace Pipeline
{
   namespace Gameplay
   {
      class LevelTileMap
      {
      private:
         std::string tile_elevation_bitmap_filename;
         std::string tile_type_bitmap_filename;
         AllegroFlare::Vec2D origin_offset;
         float ceiling_height;
         float groundlevel_height;
         float floor_height;

      protected:


      public:
         LevelTileMap();
         ~LevelTileMap();

         void set_tile_elevation_bitmap_filename(std::string tile_elevation_bitmap_filename);
         void set_tile_type_bitmap_filename(std::string tile_type_bitmap_filename);
         void set_origin_offset(AllegroFlare::Vec2D origin_offset);
         void set_ceiling_height(float ceiling_height);
         void set_groundlevel_height(float groundlevel_height);
         void set_floor_height(float floor_height);
         std::string get_tile_elevation_bitmap_filename() const;
         std::string get_tile_type_bitmap_filename() const;
         AllegroFlare::Vec2D get_origin_offset() const;
         float get_ceiling_height() const;
         float get_groundlevel_height() const;
         float get_floor_height() const;
      };
   }
}



