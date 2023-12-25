
#include <gtest/gtest.h>

#include <Pipeline/CSVToLevelLoader.hpp>


TEST(Pipeline_CSVToLevelLoaderTest, can_be_created_without_blowing_up)
{
   Pipeline::CSVToLevelLoader loader;
}


TEST(Pipeline_CSVToLevelLoaderTest, load__will_not_blow_up)
{
   Pipeline::CSVToLevelLoader loader;
   loader.set_csv_full_path("/Users/markoates/Repos/Pipeline/bin/data/levels/universe.csv");
   loader.load();
   SUCCEED();
}


TEST(Pipeline_CSVToLevelLoaderTest, load__when_loading_from_the_source_data__will_return_the_expected_values)
{
   Pipeline::CSVToLevelLoader loader;
   loader.set_csv_full_path("/Users/markoates/Repos/Pipeline/bin/data/levels/universe.csv");
   loader.load();

   EXPECT_EQ(true, loader.level_exists("level-4"));
}


