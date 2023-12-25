
#include <gtest/gtest.h>

#include <Pipeline/CSVToLevelLoader.hpp>


TEST(Pipeline_CSVToLevelLoaderTest, can_be_created_without_blowing_up)
{
   Pipeline::CSVToLevelLoader csvto_level_loader;
}


TEST(Pipeline_CSVToLevelLoaderTest, load__will_not_blow_up)
{
   Pipeline::CSVToLevelLoader csvto_level_loader;
   csvto_level_loader.set_csv_full_path("/Users/markoates/Repos/SurviveTheCity/bin/data/levels/universe.csv");
   csvto_level_loader.load();
   SUCCEED();
}


