

#include <LabyrinthOfLore/WorldMap/MultiBitmapFilenameToWorldBuilder.hpp>

#include <LabyrinthOfLore/WorldMap/BitmapTileMapLoader.hpp>
#include <LabyrinthOfLore/WorldMap/MultiBitmapTileMapLoader.hpp>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include <sstream>


namespace LabyrinthOfLore
{
namespace WorldMap
{


MultiBitmapFilenameToWorldBuilder::MultiBitmapFilenameToWorldBuilder(std::string elevation_bitmap_source_filename, std::string tile_type_bitmap_source_filename, float top_height, float ground_height)
   : elevation_bitmap_source_filename(elevation_bitmap_source_filename)
   , tile_type_bitmap_source_filename(tile_type_bitmap_source_filename)
   , top_height(top_height)
   , ground_height(ground_height)
{
}


MultiBitmapFilenameToWorldBuilder::~MultiBitmapFilenameToWorldBuilder()
{
}


std::string MultiBitmapFilenameToWorldBuilder::get_elevation_bitmap_source_filename() const
{
   return elevation_bitmap_source_filename;
}


std::string MultiBitmapFilenameToWorldBuilder::get_tile_type_bitmap_source_filename() const
{
   return tile_type_bitmap_source_filename;
}


float MultiBitmapFilenameToWorldBuilder::get_top_height() const
{
   return top_height;
}


float MultiBitmapFilenameToWorldBuilder::get_ground_height() const
{
   return ground_height;
}


LabyrinthOfLore::WorldMap::TileMap MultiBitmapFilenameToWorldBuilder::build()
{
   // could use a al_is_image_addon_installed() here
   ALLEGRO_BITMAP *elevation_bitmap_source = al_load_bitmap(elevation_bitmap_source_filename.c_str());
   ALLEGRO_BITMAP *tile_type_bitmap_source = al_load_bitmap(tile_type_bitmap_source_filename.c_str());

   if (!elevation_bitmap_source)
   {
     std::stringstream error_message;
     error_message << "[MultiBitmapFilenameToWorldBuilder::build]: error(A): was unable to load bitmap ";
     error_message << "was unable to load bitmap ";
     error_message << "\"" << elevation_bitmap_source_filename << "\" ";
     error_message << "when calling build.";
     throw std::runtime_error(error_message.str());
   }
   if (!tile_type_bitmap_source)
   {
     std::stringstream error_message;
     error_message << "[MultiBitmapFilenameToWorldBuilder::build]: error(B): was unable to load bitmap ";
     //error_message << "was unable to load bitmap ";
     error_message << "\"" << tile_type_bitmap_source_filename << "\" ";
     error_message << "when calling build.";
     throw std::runtime_error(error_message.str());
   }

   LabyrinthOfLore::WorldMap::TileMap result = LabyrinthOfLore::WorldMap::BitmapTileMapLoader(elevation_bitmap_source).load(top_height, ground_height);
   LabyrinthOfLore::WorldMap::MultiBitmapTileMapLoader(&result, tile_type_bitmap_source).load_and_process();

   // Special note!!
   // This section of the code will take any of the door tiles (type "2"), and will raise their height by 1.0 so that they stand above ground level.
   // In the future, there are many variants that could be used, but this should be added "up_the_pike" so-to-speak
   for (unsigned y=0; y<result.get_height(); y++)
      for (unsigned x=0; x<result.get_width(); x++)
      {
         if (result.get_tile(x, y).get_type() == 2)
         {
            LabyrinthOfLore::WorldMap::Tile result_tile = result.get_tile(x, y);
            result_tile.set_height(result_tile.get_height() + 1.0);
            result.set_tile(x, y, result_tile);
         }
      }

   return result;
}


} // namespace WorldMap
} // namespace LabyrinthOfLore


