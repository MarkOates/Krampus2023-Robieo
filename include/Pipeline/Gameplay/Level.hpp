#pragma once


#include <AllegroFlare/Levels/Base.hpp>
#include <AllegroFlare/Vec2D.hpp>
#include <string>


namespace Pipeline
{
   namespace Gameplay
   {
      class Level : public AllegroFlare::Levels::Base
      {
      public:
         static constexpr char* TYPE = (char*)"Pipeline/Gameplay/Level";

      private:
         std::string title;
         AllegroFlare::Vec2D tile_map_origin_offset;
         std::string tile_map_tile_elevation_bitmap_filename;
         std::string tile_map_tile_type_bitmap_filename;

      protected:


      public:
         Level();
         ~Level();

         void set_title(std::string title);
         void set_tile_map_origin_offset(AllegroFlare::Vec2D tile_map_origin_offset);
         void set_tile_map_tile_elevation_bitmap_filename(std::string tile_map_tile_elevation_bitmap_filename);
         void set_tile_map_tile_type_bitmap_filename(std::string tile_map_tile_type_bitmap_filename);
         std::string get_title() const;
         AllegroFlare::Vec2D get_tile_map_origin_offset() const;
         std::string get_tile_map_tile_elevation_bitmap_filename() const;
         std::string get_tile_map_tile_type_bitmap_filename() const;
      };
   }
}



