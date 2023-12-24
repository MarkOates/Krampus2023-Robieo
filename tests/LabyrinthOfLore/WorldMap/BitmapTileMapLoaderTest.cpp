
#include <gtest/gtest.h>

#define ASSERT_THROW_WITH_MESSAGE(code, raised_exception_type, raised_exception_message) \
   try { code; FAIL() << "Expected " # raised_exception_type; } \
   catch ( raised_exception_type const &err ) { EXPECT_EQ(err.what(), std::string( raised_exception_message )); } \
   catch (...) { FAIL() << "Expected " # raised_exception_type; }


#include <allegro5/allegro.h>

#define FLOATING_POINT_ERROR_MARGIN (0.00001f)

static void EXPECT_EQ_COLOR(ALLEGRO_COLOR expected, ALLEGRO_COLOR actual)
{
   EXPECT_EQ(expected.r, actual.r);
   EXPECT_EQ(expected.g, actual.g);
   EXPECT_EQ(expected.b, actual.b);
   EXPECT_EQ(expected.a, actual.a);
}


#include <LabyrinthOfLore/WorldMap/BitmapTileMapLoader.hpp>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>


TEST(LabyrinthOfLore_WorldMap_BitmapTileMapLoaderTest, can_be_created_without_blowing_up)
{
   LabyrinthOfLore::WorldMap::BitmapTileMapLoader bitmap_tile_map_loader;
}


TEST(LabyrinthOfLore_WorldMap_BitmapTileMapLoaderTest, load__throws_an_exception_if_there_is_a_nullptr_source_bitmap)
{
   ALLEGRO_BITMAP *source_bitmap = nullptr;

   LabyrinthOfLore::WorldMap::BitmapTileMapLoader bitmap_tile_map_loader(source_bitmap);

   std::string expected_message = "could WorldBitmap::BitmapTileMapLoader.load with a nullptr source_bitmap";
   ASSERT_THROW_WITH_MESSAGE(bitmap_tile_map_loader.load(), std::runtime_error, expected_message);
}


TEST(LabyrinthOfLore_WorldMap_BitmapTileMapLoaderTest, load__with_a_valid_bitmap__succeeds)
{
   al_init();
   al_init_image_addon();

   ALLEGRO_BITMAP *source_bitmap = al_load_bitmap("/Users/markoates/Repos/LabyrinthOfLore/bin/data/bitmaps/test_bitmap_tile_map_loader.png");
   ASSERT_NE(nullptr, source_bitmap);

   LabyrinthOfLore::WorldMap::BitmapTileMapLoader bitmap_tile_map_loader(source_bitmap);
   LabyrinthOfLore::WorldMap::TileMap actual_tile_map = bitmap_tile_map_loader.load();

   al_uninstall_system();
   SUCCEED();
}


TEST(LabyrinthOfLore_WorldMap_BitmapTileMapLoaderTest, pick_color__with_a_valid_bitmap__picks_the_color)
{
   al_init();
   al_init_image_addon();

   ALLEGRO_BITMAP *source_bitmap = al_load_bitmap("/Users/markoates/Repos/LabyrinthOfLore/bin/data/bitmaps/test_bitmap_tile_map_loader.png");
   ASSERT_NE(nullptr, source_bitmap);

   LabyrinthOfLore::WorldMap::BitmapTileMapLoader bitmap_tile_map_loader(source_bitmap);

   ALLEGRO_COLOR expected_color = al_color_html("45283c");
   ALLEGRO_COLOR actual_color = bitmap_tile_map_loader.pick_color(0, 0);

   EXPECT_EQ_COLOR(expected_color, actual_color);

   al_uninstall_system();
   SUCCEED();
}


TEST(LabyrinthOfLore_WorldMap_BitmapTileMapLoaderTest, pick_index_I_color__returns_the_expected_value)
{
   al_init();
   al_init_image_addon();

   ALLEGRO_BITMAP *source_bitmap = al_load_bitmap("/Users/markoates/Repos/LabyrinthOfLore/bin/data/bitmaps/test_bitmap_tile_map_loader.png");
   ASSERT_NE(nullptr, source_bitmap);

   LabyrinthOfLore::WorldMap::BitmapTileMapLoader bitmap_tile_map_loader(source_bitmap);

   ALLEGRO_COLOR expected_color;
   ALLEGRO_COLOR actual_color;

   expected_color = al_color_html("45283c");
   actual_color = bitmap_tile_map_loader.pick_index_null_color();
   EXPECT_EQ_COLOR(expected_color, actual_color);

   expected_color = al_color_html("ffffff");
   actual_color = bitmap_tile_map_loader.pick_index_top_color();
   EXPECT_EQ_COLOR(expected_color, actual_color);

   expected_color = al_color_html("6abe30");
   actual_color = bitmap_tile_map_loader.pick_index_ground_color();
   EXPECT_EQ_COLOR(expected_color, actual_color);

   al_uninstall_system();
   SUCCEED();
}


TEST(LabyrinthOfLore_WorldMap_BitmapTileMapLoaderTest, pick_index_G_color__returns_the_expected_higher_elevation_colors)
{
   al_init();
   al_init_image_addon();

   ALLEGRO_BITMAP *source_bitmap = al_load_bitmap("/Users/markoates/Repos/LabyrinthOfLore/bin/data/bitmaps/test_bitmap_tile_map_loader.png");
   ASSERT_NE(nullptr, source_bitmap);

   LabyrinthOfLore::WorldMap::BitmapTileMapLoader bitmap_tile_map_loader(source_bitmap);

   ALLEGRO_COLOR expected_color;
   ALLEGRO_COLOR actual_color;

   expected_color = al_color_html("5b6ee1");
   actual_color = bitmap_tile_map_loader.pick_index_gp_color();
   EXPECT_EQ_COLOR(expected_color, actual_color);

   expected_color = al_color_html("639bff");
   actual_color = bitmap_tile_map_loader.pick_index_gpp_color();
   EXPECT_EQ_COLOR(expected_color, actual_color);

   expected_color = al_color_html("5fcde4");
   actual_color = bitmap_tile_map_loader.pick_index_gppp_color();
   EXPECT_EQ_COLOR(expected_color, actual_color);

   expected_color = al_color_html("cbdbfc");
   actual_color = bitmap_tile_map_loader.pick_index_g1_color();
   EXPECT_EQ_COLOR(expected_color, actual_color);

   al_uninstall_system();
   SUCCEED();
}


TEST(LabyrinthOfLore_WorldMap_BitmapTileMapLoaderTest, pick_index_G_color__returns_the_expected_lower_elevation_colors)
{
   al_init();
   al_init_image_addon();

   ALLEGRO_BITMAP *source_bitmap = al_load_bitmap("/Users/markoates/Repos/LabyrinthOfLore/bin/data/bitmaps/test_bitmap_tile_map_loader.png");
   ASSERT_NE(nullptr, source_bitmap);

   LabyrinthOfLore::WorldMap::BitmapTileMapLoader bitmap_tile_map_loader(source_bitmap);

   ALLEGRO_COLOR expected_color;
   ALLEGRO_COLOR actual_color;

   expected_color = al_color_html("d77bba");
   actual_color = bitmap_tile_map_loader.pick_index_gm_color();
   EXPECT_EQ_COLOR(expected_color, actual_color);

   expected_color = al_color_html("d95763");
   actual_color = bitmap_tile_map_loader.pick_index_gmm_color();
   EXPECT_EQ_COLOR(expected_color, actual_color);

   expected_color = al_color_html("ac3232");
   actual_color = bitmap_tile_map_loader.pick_index_gmmm_color();
   EXPECT_EQ_COLOR(expected_color, actual_color);

   expected_color = al_color_html("663931");
   actual_color = bitmap_tile_map_loader.pick_index_gm1_color();
   EXPECT_EQ_COLOR(expected_color, actual_color);

   al_uninstall_system();
   SUCCEED();
}


TEST(LabyrinthOfLore_WorldMap_BitmapTileMapLoaderTest,
   infer_tile_map_width__returns_the_final_width_of_the__tile_map__which_should_be_2_less_than_the_source_bitmap_width)
{
   al_init();
   al_init_image_addon();

   ALLEGRO_BITMAP *source_bitmap = al_load_bitmap("/Users/markoates/Repos/LabyrinthOfLore/bin/data/bitmaps/test_bitmap_tile_map_loader.png");
   ASSERT_NE(nullptr, source_bitmap);

   LabyrinthOfLore::WorldMap::BitmapTileMapLoader bitmap_tile_map_loader(source_bitmap);

   int source_bitmap_width = al_get_bitmap_width(source_bitmap);
   int expected_tile_map_height = source_bitmap_width - 2;

   EXPECT_EQ(52, source_bitmap_width);
   EXPECT_EQ(expected_tile_map_height, bitmap_tile_map_loader.infer_tile_map_width());

   al_uninstall_system();
   SUCCEED();
}


TEST(LabyrinthOfLore_WorldMap_BitmapTileMapLoaderTest,
   infer_tile_map_height__returns_the_final_height_of_the__tile_map__which_should_be_equal_to_the_source_bitmap_height)
{
   al_init();
   al_init_image_addon();

   ALLEGRO_BITMAP *source_bitmap = al_load_bitmap("/Users/markoates/Repos/LabyrinthOfLore/bin/data/bitmaps/test_bitmap_tile_map_loader.png");
   ASSERT_NE(nullptr, source_bitmap);

   LabyrinthOfLore::WorldMap::BitmapTileMapLoader bitmap_tile_map_loader(source_bitmap);

   int source_bitmap_height = al_get_bitmap_height(source_bitmap);
   int expected_tile_map_height = source_bitmap_height;

   EXPECT_EQ(67, source_bitmap_height);
   EXPECT_EQ(expected_tile_map_height, bitmap_tile_map_loader.infer_tile_map_height());

   al_uninstall_system();
   SUCCEED();
}


TEST(LabyrinthOfLore_WorldMap_BitmapTileMapLoaderTest, load__returns_a_tile_map_with_the_expected_width_and_height)
{
   al_init();
   al_init_image_addon();

   ALLEGRO_BITMAP *source_bitmap = al_load_bitmap("/Users/markoates/Repos/LabyrinthOfLore/bin/data/bitmaps/test_bitmap_tile_map_loader.png");
   ASSERT_NE(nullptr, source_bitmap);

   LabyrinthOfLore::WorldMap::BitmapTileMapLoader loader(source_bitmap);
   LabyrinthOfLore::WorldMap::TileMap tile_map = loader.load();

   EXPECT_EQ(50, tile_map.get_width());
   EXPECT_EQ(67, tile_map.get_height());

   al_uninstall_system();
   SUCCEED();
}


TEST(LabyrinthOfLore_WorldMap_BitmapTileMapLoaderTest,
   colors_are_equal__returns_true_if_colors_are_the_same__and_false_if_they_are_different)
{
   al_init();
   al_init_image_addon();

   LabyrinthOfLore::WorldMap::BitmapTileMapLoader loader(nullptr);

   EXPECT_EQ(true, loader.colors_are_equal(al_color_html("000000"), al_color_html("000000")));
   EXPECT_EQ(true, loader.colors_are_equal(al_color_html("ac3232"), al_color_html("ac3232")));
   EXPECT_EQ(true, loader.colors_are_equal(al_color_html("6abe30"), al_color_html("6abe30")));
   EXPECT_EQ(true, loader.colors_are_equal(al_color_html("ffffff"), al_color_html("ffffff")));

   EXPECT_EQ(false, loader.colors_are_equal(al_color_html("ac3232"), al_color_html("ac3231")));
   EXPECT_EQ(false, loader.colors_are_equal(al_color_html("ac3232"), al_color_html("ac3132")));
   EXPECT_EQ(false, loader.colors_are_equal(al_color_html("ac3232"), al_color_html("ab3232")));

   al_uninstall_system();
   SUCCEED();
}


TEST(LabyrinthOfLore_WorldMap_BitmapTileMapLoaderTest, load__correctly_picks_out_top_tiles)
{
   al_init();
   al_init_image_addon();

   ALLEGRO_BITMAP *source_bitmap = al_load_bitmap("/Users/markoates/Repos/LabyrinthOfLore/bin/data/bitmaps/test_bitmap_tile_map_loader.png");
   ASSERT_NE(nullptr, source_bitmap);

   LabyrinthOfLore::WorldMap::BitmapTileMapLoader loader(source_bitmap);
   LabyrinthOfLore::WorldMap::TileMap tile_map = loader.load();

   EXPECT_EQ(1, tile_map.get_tile(13, 5).get_type());
   EXPECT_EQ(4.0, tile_map.get_tile(13, 5).get_height());
   EXPECT_EQ(1, tile_map.get_tile(16, 22).get_type());
   EXPECT_EQ(4.0, tile_map.get_tile(16, 22).get_height());
   EXPECT_EQ(1, tile_map.get_tile(7, 17).get_type());
   EXPECT_EQ(4.0, tile_map.get_tile(7, 17).get_height());

   al_uninstall_system();
   SUCCEED();
}


TEST(LabyrinthOfLore_WorldMap_BitmapTileMapLoaderTest, load__correctly_picks_out_ground_tiles)
{
   al_init();
   al_init_image_addon();

   ALLEGRO_BITMAP *source_bitmap = al_load_bitmap("/Users/markoates/Repos/LabyrinthOfLore/bin/data/bitmaps/test_bitmap_tile_map_loader.png");
   ASSERT_NE(nullptr, source_bitmap);
LabyrinthOfLore::WorldMap::BitmapTileMapLoader loader(source_bitmap);
   LabyrinthOfLore::WorldMap::TileMap tile_map = loader.load();

   EXPECT_EQ(1, tile_map.get_tile(13, 14).get_type());
   EXPECT_EQ(2.0, tile_map.get_tile(13, 14).get_height());
   EXPECT_EQ(1, tile_map.get_tile(16, 17).get_type());
   EXPECT_EQ(2.0, tile_map.get_tile(16, 17).get_height());

   al_uninstall_system();
   SUCCEED();
}


TEST(LabyrinthOfLore_WorldMap_BitmapTileMapLoaderTest, load__correctly_picks_out_ascending_elevation_tiles)
{
   al_init();
   al_init_image_addon();

   ALLEGRO_BITMAP *source_bitmap = al_load_bitmap("/Users/markoates/Repos/LabyrinthOfLore/bin/data/bitmaps/test_bitmap_tile_map_loader.png");
   ASSERT_NE(nullptr, source_bitmap);

   LabyrinthOfLore::WorldMap::BitmapTileMapLoader loader(source_bitmap);
   LabyrinthOfLore::WorldMap::TileMap tile_map = loader.load();

   EXPECT_EQ(1, tile_map.get_tile(14, 19).get_type());
   EXPECT_EQ(2.25, tile_map.get_tile(14, 19).get_height());
   EXPECT_EQ(1, tile_map.get_tile(15, 21).get_type());
   EXPECT_EQ(2.5, tile_map.get_tile(15, 21).get_height());
   EXPECT_EQ(1, tile_map.get_tile(15, 24).get_type());
   EXPECT_EQ(2.75, tile_map.get_tile(15, 24).get_height());
   EXPECT_EQ(1, tile_map.get_tile(15, 25).get_type());
   EXPECT_EQ(3.0, tile_map.get_tile(15, 25).get_height());


   al_uninstall_system();
   SUCCEED();
}


TEST(LabyrinthOfLore_WorldMap_BitmapTileMapLoaderTest, load__correctly_picks_out_descending_elevation_tiles)
{
   al_init();
   al_init_image_addon();

   ALLEGRO_BITMAP *source_bitmap = al_load_bitmap("/Users/markoates/Repos/LabyrinthOfLore/bin/data/bitmaps/test_bitmap_tile_map_loader.png");
   ASSERT_NE(nullptr, source_bitmap);

   LabyrinthOfLore::WorldMap::BitmapTileMapLoader loader(source_bitmap);
   LabyrinthOfLore::WorldMap::TileMap tile_map = loader.load();

   EXPECT_EQ(1, tile_map.get_tile(9, 13).get_type());
   EXPECT_EQ(1.75, tile_map.get_tile(9, 13).get_height());
   EXPECT_EQ(1, tile_map.get_tile(9, 10).get_type());
   EXPECT_EQ(1.5, tile_map.get_tile(9, 10).get_height());
   EXPECT_EQ(1, tile_map.get_tile(7, 8).get_type());
   EXPECT_EQ(1.25, tile_map.get_tile(7, 8).get_height());
   EXPECT_EQ(1, tile_map.get_tile(6, 14).get_type());
   EXPECT_EQ(1.0, tile_map.get_tile(6, 14).get_height());


   al_uninstall_system();
   SUCCEED();
}


