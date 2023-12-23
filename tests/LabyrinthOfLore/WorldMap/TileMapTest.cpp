
#include <gtest/gtest.h>

#include <LabyrinthOfLore/WorldMap/TileMap.hpp>

#define ASSERT_THROW_WITH_MESSAGE(code, raised_exception_type, raised_exception_message) \
   try { code; FAIL() << "Expected " # raised_exception_type; } \
   catch ( raised_exception_type const &err ) { EXPECT_EQ(err.what(), std::string( raised_exception_message )); } \
   catch (...) { FAIL() << "Expected " # raised_exception_type; }


TEST(LabyrinthOfLore_WorldMap_TileMapTest, can_be_created_without_blowing_up)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
}


TEST(LabyrinthOfLore_WorldMap_TileMapTest, get_tile__when_requesting_a_tile_outside_of_bounds__returns_a_minus1_type_tile)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;

   ASSERT_EQ(-1, tile_map.get_tile(-1, -1).get_type());
}


TEST(LabyrinthOfLore_WorldMap_TileMapTest, ceiling_height__has_getters_and_setters_and_had_the_expected_default)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;

   ASSERT_EQ(100.0f, tile_map.get_ceiling_height());

   tile_map.set_ceiling_height(30.0);
   ASSERT_EQ(30.0f, tile_map.get_ceiling_height());
}

TEST(LabyrinthOfLore_WorldMap_TileMapTest, ceiling_height__when_attempting_to_set_to_a_value_less_than_one__raises_an_exception)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;

   std::vector<float> invalid_ceiling_height_values = { 0.0f, 0.999f, -1.0f, -999.0f };

   std::string expected_message = "Cannot set set_ceiling_height in TileMap to a value less than 1.0f.";
   for (auto &invalid_ceiling_height_value : invalid_ceiling_height_values)
   {
      ASSERT_THROW_WITH_MESSAGE(tile_map.set_ceiling_height(invalid_ceiling_height_value), std::runtime_error, expected_message)
   }
}

TEST(LabyrinthOfLore_WorldMap_TileMapTest, resize__will_fill_the_tiles_with_the_default_tile)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   tile_map.resize(10, 10);

   LabyrinthOfLore::WorldMap::Tile default_tile;
   ASSERT_EQ(default_tile.get_type(), tile_map.get_tile(3, 7).get_type());
   ASSERT_EQ(default_tile.get_height(), tile_map.get_tile(3, 7).get_height());
}


TEST(LabyrinthOfLore_WorldMap_TileMapTest, resize__will_fill_the_tiles_with_the_passed_tile)
{
   LabyrinthOfLore::WorldMap::TileMap tile_map;
   LabyrinthOfLore::WorldMap::Tile tile(7, 12.0);

   tile_map.resize(10, 10, tile);

   LabyrinthOfLore::WorldMap::Tile expected_tile = tile;
   LabyrinthOfLore::WorldMap::Tile actual_tile = tile_map.get_tile(3, 7);

   ASSERT_EQ(tile.get_type(), actual_tile.get_type());
   ASSERT_EQ(tile.get_height(), actual_tile.get_height());
}


