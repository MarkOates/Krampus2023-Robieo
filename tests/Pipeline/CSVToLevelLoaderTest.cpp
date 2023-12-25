
#include <gtest/gtest.h>

#include <Pipeline/CSVToLevelLoader.hpp>


TEST(Pipeline_CSVToLevelLoaderTest, can_be_created_without_blowing_up)
{
   Pipeline::CSVToLevelLoader loader;
}


TEST(Pipeline_CSVToLevelLoaderTest, load__will_not_blow_up)
{
   Pipeline::CSVToLevelLoader loader;
   loader.set_csv_full_path("/Users/markoates/Repos/Pipeline/tests/fixtures/levels/universe-fixture.csv");
   loader.load();
   SUCCEED();
}


TEST(Pipeline_CSVToLevelLoaderTest, load__when_loading_from_the_source_data__will_create_the_expected_records)
{
   Pipeline::CSVToLevelLoader loader;
   loader.set_csv_full_path("/Users/markoates/Repos/Pipeline/bin/data/levels/universe.csv");
   loader.load();

   EXPECT_EQ(true, loader.level_exists("level-4"));
}


TEST(Pipeline_CSVToLevelLoaderTest, load__when_loading_from_the_source_data__will_load_records_with_the_expected_data)
{
   Pipeline::CSVToLevelLoader loader;
   loader.set_csv_full_path("/Users/markoates/Repos/Pipeline/tests/fixtures/levels/universe-fixture.csv");
   loader.load();

   ASSERT_EQ(true, loader.level_exists("level-4"));

   Pipeline::Gameplay::Level actual_level = loader.find_level("level-4");

   EXPECT_EQ("World 4", actual_level.get_title());
   EXPECT_EQ("level-4-09.obj", actual_level.get_world_model_obj_filename());
   EXPECT_EQ("level-4-09.png", actual_level.get_world_model_texture_filename());

   EXPECT_EQ("the_cave.png", actual_level.get_tile_map_tile_elevation_bitmap_filename());
   EXPECT_EQ("the_cave-type.png", actual_level.get_tile_map_tile_type_bitmap_filename());

   EXPECT_EQ(AllegroFlare::Vec2D(22, 28), actual_level.get_tile_map_origin_offset());
   EXPECT_EQ(10.0, actual_level.get_tile_map_ceiling_height());
   EXPECT_EQ(0.0, actual_level.get_tile_map_groundlevel_height());
   EXPECT_EQ(-2.0, actual_level.get_tile_map_floor_height());

   EXPECT_EQ("robot-holly_jolly", actual_level.get_song_to_perform_identifier());
   EXPECT_EQ(15.0, actual_level.get_song_to_perform_duration_sec());
}


TEST(Pipeline_CSVToLevelLoaderTest, load__on_production_csv__will_not_blow_up)
{
   Pipeline::CSVToLevelLoader loader;
   loader.set_csv_full_path("/Users/markoates/Repos/Pipeline/bin/data/levels/universe.csv");
   loader.load();
   SUCCEED();
}


