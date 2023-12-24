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
         std::string world_model_obj_filename;
         std::string world_model_texture_filename;
         std::string tile_map_tile_elevation_bitmap_filename;
         std::string tile_map_tile_type_bitmap_filename;
         AllegroFlare::Vec2D tile_map_origin_offset;
         float tile_map_ceiling_height;
         float tile_map_groundlevel_height;
         float tile_map_floor_height;
         std::string song_to_perform_identifier;
         float song_to_perform_duration;

      protected:


      public:
         Level();
         ~Level();

         void set_title(std::string title);
         void set_world_model_obj_filename(std::string world_model_obj_filename);
         void set_world_model_texture_filename(std::string world_model_texture_filename);
         void set_tile_map_tile_elevation_bitmap_filename(std::string tile_map_tile_elevation_bitmap_filename);
         void set_tile_map_tile_type_bitmap_filename(std::string tile_map_tile_type_bitmap_filename);
         void set_tile_map_origin_offset(AllegroFlare::Vec2D tile_map_origin_offset);
         void set_tile_map_ceiling_height(float tile_map_ceiling_height);
         void set_tile_map_groundlevel_height(float tile_map_groundlevel_height);
         void set_tile_map_floor_height(float tile_map_floor_height);
         void set_song_to_perform_identifier(std::string song_to_perform_identifier);
         void set_song_to_perform_duration(float song_to_perform_duration);
         std::string get_title() const;
         std::string get_world_model_obj_filename() const;
         std::string get_world_model_texture_filename() const;
         std::string get_tile_map_tile_elevation_bitmap_filename() const;
         std::string get_tile_map_tile_type_bitmap_filename() const;
         AllegroFlare::Vec2D get_tile_map_origin_offset() const;
         float get_tile_map_ceiling_height() const;
         float get_tile_map_groundlevel_height() const;
         float get_tile_map_floor_height() const;
         std::string get_song_to_perform_identifier() const;
         float get_song_to_perform_duration() const;
      };
   }
}



