#pragma once


#include <AllegroFlare/Levels/Base.hpp>
#include <Pipeline/Gameplay/LevelTileMap.hpp>
#include <string>
#include <vector>


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
         std::vector<Pipeline::Gameplay::LevelTileMap> tile_maps;
         std::string background_music_identifier;
         std::string song_to_perform_identifier;
         float song_to_perform_duration_sec;

      protected:


      public:
         Level();
         ~Level();

         void set_title(std::string title);
         void set_world_model_obj_filename(std::string world_model_obj_filename);
         void set_world_model_texture_filename(std::string world_model_texture_filename);
         void set_tile_maps(std::vector<Pipeline::Gameplay::LevelTileMap> tile_maps);
         void set_background_music_identifier(std::string background_music_identifier);
         void set_song_to_perform_identifier(std::string song_to_perform_identifier);
         void set_song_to_perform_duration_sec(float song_to_perform_duration_sec);
         std::string get_title() const;
         std::string get_world_model_obj_filename() const;
         std::string get_world_model_texture_filename() const;
         std::vector<Pipeline::Gameplay::LevelTileMap> get_tile_maps() const;
         std::string get_background_music_identifier() const;
         std::string get_song_to_perform_identifier() const;
         float get_song_to_perform_duration_sec() const;
         std::vector<Pipeline::Gameplay::LevelTileMap> &get_tile_maps_ref();
      };
   }
}



